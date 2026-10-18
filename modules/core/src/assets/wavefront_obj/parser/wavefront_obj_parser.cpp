#include "assets/wavefront_obj/parser/wavefront_obj_parser.hpp"

#include "assets/wavefront_obj/parser/wavefront_obj_parser_errors.hpp"
#include "assets/wavefront_obj/parser/wavefront_obj_parser_types.hpp"
#include "assets/wavefront_obj/wavefront_obj_asset.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <limits>
#include <optional>
#include <string_view>
#include <system_error>
#include <utility>
#include <variant>

namespace sde
{
namespace
{
enum class Action : std::uint8_t
{
    NONE,

    BEGIN_POSITION,
    BEGIN_TEXTURE_COORDINATE,
    BEGIN_NORMAL,
    BEGIN_FACE,

    APPEND_COORDINATE,
    APPEND_FACE_VERTEX,

    COMMIT_VERTEX,
    COMMIT_TEXTURE_COORDINATE,
    COMMIT_NORMAL,
    COMMIT_FACE,
};

enum class State : std::uint8_t
{
    START,

    POSITION_0,
    POSITION_1,
    POSITION_2,
    POSITION_3,
    POSITION_4,

    TEXCOORD_0,
    TEXCOORD_1,
    TEXCOORD_2,
    TEXCOORD_3,

    NORMAL_0,
    NORMAL_1,
    NORMAL_2,
    NORMAL_3,

    FACE_0,
    FACE_1,
    FACE_2,
    FACE_3_PLUS,

    IGNORE,

    COUNT,
};

struct Transition
{
    State next_state{};
    Action action{};
};

using TransitionTable =
    std::array<std::array<std::optional<Transition>, std::to_underlying(WavefrontOBJToken::TokenKind::COUNT)>, std::to_underlying(State::COUNT)>;

constexpr auto create_transition_table() -> TransitionTable
{
    TransitionTable transition_table{};

    auto add_transition = [&](State from, WavefrontOBJToken::TokenKind token, State to, Action action = Action::NONE) -> void {
        transition_table[std::to_underlying(from)][std::to_underlying(token)] = Transition{ .next_state = to, .action = action };
    };

    // Geometric vertices transitions
    // -----------------------------------------------------------------------------------------------------------
    {
        add_transition(State::START, WavefrontOBJToken::TokenKind::GEOMETRIC_VERTEX, State::POSITION_0, Action::BEGIN_POSITION);

        for (const auto token_kind : { WavefrontOBJToken::TokenKind::INTEGER, WavefrontOBJToken::TokenKind::FLOAT }) {
            add_transition(State::POSITION_0, token_kind, State::POSITION_1, Action::APPEND_COORDINATE);
            add_transition(State::POSITION_1, token_kind, State::POSITION_2, Action::APPEND_COORDINATE);
            add_transition(State::POSITION_2, token_kind, State::POSITION_3, Action::APPEND_COORDINATE);
            add_transition(State::POSITION_3, token_kind, State::POSITION_4, Action::APPEND_COORDINATE);
        }

        // Geometric vertices may not contain a homogeneous coordinate, so there is two transitions
        add_transition(State::POSITION_3, WavefrontOBJToken::TokenKind::END_OF_LINE, State::START, Action::COMMIT_VERTEX);
        add_transition(State::POSITION_4, WavefrontOBJToken::TokenKind::END_OF_LINE, State::START, Action::COMMIT_VERTEX);
    }

    // Texture vertices transitions
    // -----------------------------------------------------------------------------------------------------------
    {
        add_transition(State::START, WavefrontOBJToken::TokenKind::TEXTURE_VERTEX, State::TEXCOORD_0, Action::BEGIN_TEXTURE_COORDINATE);

        for (const auto token_kind : { WavefrontOBJToken::TokenKind::INTEGER, WavefrontOBJToken::TokenKind::FLOAT }) {
            add_transition(State::TEXCOORD_0, token_kind, State::TEXCOORD_1, Action::APPEND_COORDINATE);
            add_transition(State::TEXCOORD_1, token_kind, State::TEXCOORD_2, Action::APPEND_COORDINATE);
            add_transition(State::TEXCOORD_2, token_kind, State::TEXCOORD_3, Action::APPEND_COORDINATE);
        }

        for (const auto state : { State::TEXCOORD_1, State::TEXCOORD_2, State::TEXCOORD_3 }) {
            add_transition(state, WavefrontOBJToken::TokenKind::END_OF_LINE, State::START, Action::COMMIT_TEXTURE_COORDINATE);
        }
    }

    // Vertex normals transitions
    // ------------------------------------------------------------------------------------------------------------
    {
        add_transition(State::START, WavefrontOBJToken::TokenKind::VERTEX_NORMAL, State::NORMAL_0, Action::BEGIN_NORMAL);

        for (const auto token_kind : { WavefrontOBJToken::TokenKind::INTEGER, WavefrontOBJToken::TokenKind::FLOAT }) {
            add_transition(State::NORMAL_0, token_kind, State::NORMAL_1, Action::APPEND_COORDINATE);
            add_transition(State::NORMAL_1, token_kind, State::NORMAL_2, Action::APPEND_COORDINATE);
            add_transition(State::NORMAL_2, token_kind, State::NORMAL_3, Action::APPEND_COORDINATE);
        }
        add_transition(State::NORMAL_3, WavefrontOBJToken::TokenKind::END_OF_LINE, State::START, Action::COMMIT_NORMAL);
    }

    // Face transitions
    // -----------------------------------------------------------------------------------------------------------
    {
        add_transition(State::START, WavefrontOBJToken::TokenKind::FACE, State::FACE_0, Action::BEGIN_FACE);

        // OBJ faces may contain position-only indices (e.g., "f 1 2 3").
        // Since tokenization is context-independent, these are classified as INTEGER rather than FACE_REFERENCE.
        // Because of this we need to accept both token kinds in face states.
        for (const auto token_kind : { WavefrontOBJToken::TokenKind::FACE_REFERENCE, WavefrontOBJToken::TokenKind::INTEGER }) {
            add_transition(State::FACE_0, token_kind, State::FACE_1, Action::APPEND_FACE_VERTEX);
            add_transition(State::FACE_1, token_kind, State::FACE_2, Action::APPEND_FACE_VERTEX);
            add_transition(State::FACE_2, token_kind, State::FACE_3_PLUS, Action::APPEND_FACE_VERTEX);
            add_transition(State::FACE_3_PLUS, token_kind, State::FACE_3_PLUS, Action::APPEND_FACE_VERTEX);
        }

        add_transition(State::FACE_3_PLUS, WavefrontOBJToken::TokenKind::END_OF_LINE, State::START, Action::COMMIT_FACE);
    }

    // Skip empty lines
    {
        add_transition(State::START, WavefrontOBJToken::TokenKind::END_OF_LINE, State::START);
    }

    // Ignore unsupported statements
    {
        add_transition(State::START, WavefrontOBJToken::TokenKind::UNSUPPORTED_IDENTIFIER, State::IGNORE);

        for (std::size_t token_kind = 0z; token_kind < std::to_underlying(WavefrontOBJToken::TokenKind::COUNT); ++token_kind) {
            add_transition(State::IGNORE, static_cast<WavefrontOBJToken::TokenKind>(token_kind), State::IGNORE);
        }

        add_transition(State::IGNORE, WavefrontOBJToken::TokenKind::END_OF_LINE, State::START);
    }

    return transition_table;
}

template <class T>
auto parse_number(std::string_view text) -> std::expected<T, WavefrontOBJTokenizeError::Errc>
{
    // std::from_chars doesn't accept a leading '+'.
    if (text.starts_with('+')) {
        text.remove_prefix(1);
    }

    T value{};
    const auto [ptr, ec] = std::from_chars(text.data(), text.data() + text.size(), value);

    if (ec == std::errc::result_out_of_range) {
        return std::unexpected(WavefrontOBJTokenizeError::Errc::NUMBER_OUT_OF_RANGE);
    }

    if (ec != std::errc{} || ptr != text.data() + text.size()) {
        return std::unexpected(WavefrontOBJTokenizeError::Errc::INVALID_NUMBER);
    }

    return value;
}

auto parse_face_reference(std::string_view text) -> std::expected<WavefrontOBJToken::Face, WavefrontOBJTokenizeError::Errc>
{
    auto parse_index = [](std::string_view part) -> std::expected<std::int64_t, WavefrontOBJTokenizeError::Errc> {
        if (part.empty()) {
            return std::unexpected(WavefrontOBJTokenizeError::Errc::INVALID_FACE_REFERENCE);
        }

        return parse_number<std::int64_t>(part).transform_error([](WavefrontOBJTokenizeError::Errc error) -> WavefrontOBJTokenizeError::Errc {
            return error == WavefrontOBJTokenizeError::Errc::INVALID_NUMBER ? WavefrontOBJTokenizeError::Errc::INVALID_FACE_REFERENCE : error;
        });
    };

    const auto first_slash = text.find('/');
    if (first_slash == std::string_view::npos) {
        return std::unexpected(WavefrontOBJTokenizeError::Errc::INVALID_FACE_REFERENCE);
    }

    auto vertex = parse_index(text.substr(0, first_slash));
    if (!vertex) {
        return std::unexpected(vertex.error());
    }

    WavefrontOBJToken::Face result{ .vertex_index = *vertex };

    const auto second_slash = text.find('/', first_slash + 1);

    if (second_slash == std::string_view::npos) {
        // v/vt
        auto texture = parse_index(text.substr(first_slash + 1));
        if (!texture) {
            return std::unexpected(texture.error());
        }
        result.texture_vertex_index = *texture;
    }
    else {
        // v//vn or v/vt/vn
        if (text.find('/', second_slash + 1) != std::string_view::npos) {
            return std::unexpected(WavefrontOBJTokenizeError::Errc::INVALID_FACE_REFERENCE);
        }

        if (second_slash != first_slash + 1) {
            auto texture = parse_index(text.substr(first_slash + 1, second_slash - first_slash - 1));
            if (!texture) {
                return std::unexpected(texture.error());
            }
            result.texture_vertex_index = *texture;
        }

        auto normal = parse_index(text.substr(second_slash + 1));
        if (!normal) {
            return std::unexpected(normal.error());
        }
        result.vertex_normal_index = *normal;
    }

    return result;
}

[[maybe_unused]] auto classify_lexeme(std::string_view lexeme, std::size_t line, std::size_t column)
    -> std::expected<WavefrontOBJToken, WavefrontOBJTokenizeError>
{
    using std::ranges::all_of;
    using std::ranges::any_of;

    auto create_token = [&](WavefrontOBJToken::TokenKind kind, WavefrontOBJToken::TokenValue value = {}) -> WavefrontOBJToken {
        return { .kind = kind, .value = value, .lexeme = lexeme, .line = line, .column = column };
    };

    auto create_error = [&](WavefrontOBJTokenizeError::Errc code) -> std::unexpected<WavefrontOBJTokenizeError> {
        return std::unexpected(WavefrontOBJTokenizeError{ .code = code, .lexeme = lexeme, .line = line, .column = column });
    };

    if (lexeme == "v") {
        return create_token(WavefrontOBJToken::TokenKind::GEOMETRIC_VERTEX);
    }
    if (lexeme == "vt") {
        return create_token(WavefrontOBJToken::TokenKind::TEXTURE_VERTEX);
    }
    if (lexeme == "vn") {
        return create_token(WavefrontOBJToken::TokenKind::VERTEX_NORMAL);
    }
    if (lexeme == "f") {
        return create_token(WavefrontOBJToken::TokenKind::FACE);
    }

    auto is_digit = [](char c) -> bool {
        return c >= '0' && c <= '9';
    };

    const bool has_digit = any_of(lexeme, is_digit);

    if (lexeme.contains('/')) {
        // Distinguish index references from ordinary paths and identifiers.
        auto is_reference_char = [&](char c) -> bool {
            return is_digit(c) || c == '+' || c == '-' || c == '/';
        };

        if (has_digit && all_of(lexeme, is_reference_char)) {
            auto value = parse_face_reference(lexeme);
            if (!value) {
                return create_error(value.error());
            }
            return create_token(WavefrontOBJToken::TokenKind::FACE_REFERENCE, *value);
        }
    }
    else {
        auto is_number_char = [&](char c) -> bool {
            return is_digit(c) || c == '+' || c == '-' || c == '.' || c == 'e' || c == 'E';
        };

        if (has_digit && all_of(lexeme, is_number_char)) {
            if (lexeme.find_first_of(".eE") != std::string_view::npos) {
                auto value = parse_number<double>(lexeme);
                if (!value) {
                    return create_error(value.error());
                }
                return create_token(WavefrontOBJToken::TokenKind::FLOAT, *value);
            }

            auto value = parse_number<std::int64_t>(lexeme);
            if (!value) {
                return create_error(value.error());
            }
            return create_token(WavefrontOBJToken::TokenKind::INTEGER, *value);
        }
    }

    return create_token(WavefrontOBJToken::TokenKind::UNSUPPORTED_IDENTIFIER);
}

auto resolve_index(std::int64_t index, std::size_t count) -> std::optional<std::size_t>
{
    if (index == 0 || count > static_cast<std::size_t>(std::numeric_limits<std::int64_t>::max())) {
        return std::nullopt;
    }

    const auto size = static_cast<std::int64_t>(count);

    // Positive indices are 1-based; negative indices are relative to
    // the end of the corresponding attribute array.
    const auto resolved = index > 0 ? index - 1 : size + index;

    if (resolved < 0 || resolved >= size) {
        return std::nullopt;
    }

    return static_cast<std::size_t>(resolved);
}

struct ParserContext
{
    WavefrontOBJAsset data;

    // Temporary storage for v, vt and vn statements.
    std::array<float, 4> coordinates{};
    std::size_t coordinate_count = 0;

    // Face currently being constructed.
    WavefrontOBJAsset::Face current_face;
};

auto execute_action(Action action, const WavefrontOBJToken& token, ParserContext& context) -> std::expected<void, WavefrontOBJParseError>
{
    auto create_error = [&](WavefrontOBJParseError::Errc code) -> std::unexpected<WavefrontOBJParseError> {
        return std::unexpected(WavefrontOBJParseError{ .code = code, .line = token.line, .column = token.column });
    };

    switch (action) {
    case Action::NONE:
    {
        return {};
    }
    case Action::BEGIN_POSITION:
        [[fallthrough]];
    case Action::BEGIN_TEXTURE_COORDINATE:
        [[fallthrough]];
    case Action::BEGIN_NORMAL:
    {
        context.coordinates = { 0.f, 0.f, 0.f, action == Action::BEGIN_POSITION ? 1.f : 0.f };
        context.coordinate_count = 0;
        return {};
    }

    case Action::APPEND_COORDINATE:
    {
        double value = std::numeric_limits<double>::quiet_NaN();

        if (const auto* integer = std::get_if<std::int64_t>(&token.value)) {
            value = static_cast<double>(*integer);
        }
        else if (const auto* floating = std::get_if<double>(&token.value)) {
            value = *floating;
        }
        else {
            return create_error(WavefrontOBJParseError::Errc::INVALID_ACTION_TOKEN);
        }

        if (!std::isfinite(value)) {
            return create_error(WavefrontOBJParseError::Errc::NON_FINITE_COORDINATE);
        }

        if (std::abs(value) > std::numeric_limits<double>::max()) {
            return create_error(WavefrontOBJParseError::Errc::COORDINATE_OUT_OF_RANGE);
        }

        if (context.coordinate_count >= context.coordinates.size()) {
            return create_error(WavefrontOBJParseError::Errc::INVALID_ACTION_TOKEN);
        }

        context.coordinates[context.coordinate_count++] = static_cast<float>(value);
        return {};
    }

    case Action::COMMIT_VERTEX:
    {
        context.data.positions.emplace_back(context.coordinates[0], context.coordinates[1], context.coordinates[2], context.coordinates[3]);
        return {};
    }

    case Action::COMMIT_TEXTURE_COORDINATE:
    {
        context.data.texcoords.emplace_back(context.coordinates[0], context.coordinates[1], context.coordinates[2]);
        return {};
    }

    case Action::COMMIT_NORMAL:
    {
        context.data.normals.emplace_back(context.coordinates[0], context.coordinates[1], context.coordinates[2]);
        return {};
    }

    case Action::BEGIN_FACE:
    {
        context.current_face.vertices.clear();
        return {};
    }
    case Action::APPEND_FACE_VERTEX:
    {
        WavefrontOBJToken::Face face;

        if (const auto* parsed = std::get_if<WavefrontOBJToken::Face>(&token.value)) {
            face = *parsed;
        }
        else if (const auto* index = std::get_if<std::int64_t>(&token.value)) {
            face.vertex_index = *index;
        }
        else {
            return create_error(WavefrontOBJParseError::Errc::INVALID_ACTION_TOKEN);
        }

        const auto position = resolve_index(face.vertex_index, context.data.positions.size());

        if (!position) {
            return create_error(WavefrontOBJParseError::Errc::INVALID_VERTEX_INDEX);
        }

        WavefrontOBJAsset::Face::Vertex vertex{};
        vertex.position_index = *position;

        if (face.texture_vertex_index) {
            const auto texture = resolve_index(*face.texture_vertex_index, context.data.texcoords.size());

            if (!texture) {
                return create_error(WavefrontOBJParseError::Errc::INVALID_TEXTURE_INDEX);
            }

            vertex.texcoord_index = *texture;
        }

        if (face.vertex_normal_index) {
            const auto normal = resolve_index(*face.vertex_normal_index, context.data.normals.size());

            if (!normal) {
                return create_error(WavefrontOBJParseError::Errc::INVALID_NORMAL_INDEX);
            }

            vertex.normal_index = *normal;
        }

        context.current_face.vertices.push_back(vertex);
        return {};
    }

    case Action::COMMIT_FACE:
    {
        context.data.faces.push_back(std::move(context.current_face));
        return {};
    }
    default:
    {
        assert(false);
    }
    }

    return create_error(WavefrontOBJParseError::Errc::INVALID_ACTION_TOKEN);
}
} // namespace

auto WavefrontOBJParser::parse_wavefront_obj(std::string_view source) -> std::expected<WavefrontOBJAsset, WavefrontOBJError>
{
    static constexpr auto TRANSITION_TABLE = create_transition_table();

    WavefrontOBJParser tokenizer(source);
    ParserContext context;
    State state = State::START;

    while (true) {
        auto token = tokenizer.next();
        if (!token) {
            return std::unexpected(token.error());
        }

        if (token->kind == WavefrontOBJToken::TokenKind::END_OF_FILE) {
            if (state != State::START) {
                return std::unexpected(
                    WavefrontObjSyntaxError{ .code = WavefrontObjSyntaxError::Errc::UNEXPECTED_EOF, .line = token->line, .column = token->column });
            }

            break;
        }

        const auto& transition = TRANSITION_TABLE[std::to_underlying(state)][std::to_underlying(token->kind)];

        if (!transition) {
            return std::unexpected(
                WavefrontObjSyntaxError{ .code = WavefrontObjSyntaxError::Errc::UNEXPECTED_TOKEN, .line = token->line, .column = token->column });
        }

        if (auto result = execute_action(transition->action, *token, context); !result) {
            return std::unexpected(result.error());
        }

        state = transition->next_state;
    }

    return std::move(context.data);
}

WavefrontOBJParser::WavefrontOBJParser(std::string_view _source)
    : source(_source)
{}

auto WavefrontOBJParser::next() -> std::expected<WavefrontOBJToken, WavefrontOBJTokenizeError>
{
    auto boundary = [&](WavefrontOBJToken::TokenKind kind) -> WavefrontOBJToken {
        return { .kind = kind, .value = {}, .lexeme = {}, .line = line, .column = column };
    };

    auto is_whitespace = [](char c) -> bool {
        return c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f';
    };

    while (position < source.size()) {
        const char c = source[position];

        if (c == '\n') {
            auto result = boundary(WavefrontOBJToken::TokenKind::END_OF_LINE);

            ++position;
            ++line;
            column = 1;
            unterminated_line = false;

            return result;
        }

        if (c == '#') {
            while (position < source.size() && source[position] != '\n') {
                ++position;
                ++column;
            }
            continue;
        }

        if (is_whitespace(c)) {
            ++position;
            ++column;
            continue;
        }

        const auto start = position;
        const auto start_column = column;

        while (position < source.size()) {
            const char ch = source[position];

            if (ch == '\n' || ch == '#' || is_whitespace(ch)) {
                break;
            }

            ++position;
            ++column;
        }

        unterminated_line = true;

        return classify_lexeme(source.substr(start, position - start), line, start_column);
    }

    // Emit a synthetic EOL when the final line has no trailing newline.
    if (unterminated_line) {
        unterminated_line = false;

        auto result = boundary(WavefrontOBJToken::TokenKind::END_OF_LINE);
        ++line;
        column = 1;

        return result;
    }

    return boundary(WavefrontOBJToken::TokenKind::END_OF_FILE);
}
} // namespace sde
