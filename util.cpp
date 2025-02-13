#define SDL_ERROR(v) if (!v) { printf("ERROR: %s\n", SDL_GetError()); return 1; }
#define INFO(fmt, ...) printf(("Info [%s]: " + string(fmt)).c_str(), __PRETTY_FUNCTION__, __VA_ARGS__)
#define INFOLN(fmt) INFO("%s\n", fmt)
#define ERROR(fmt, ...) printf(("Error [%s]: " + string(fmt)).c_str(), __PRETTY_FUNCTION__, __VA_ARGS__)
#define ERRORLN(fmt) ERROR("%s\n", fmt)
#define WARNING(fmt, ...) printf(("Warning [%s]: " + string(fmt)).c_str(), __PRETTY_FUNCTION__, __VA_ARGS__)
#define WARNINGLN(fmt) WARNING("%s\n", fmt)

auto read_file(std::string_view path) -> std::string {
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(path.data());
    stream.exceptions(std::ios_base::badbit);

    if (not stream) {
        throw std::ios_base::failure("file does not exist");
    }
    
    auto out = std::string();
    auto buf = std::string(read_size, '\0');
    while (stream.read(& buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}
