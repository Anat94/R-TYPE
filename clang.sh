find ./src \( -name '*.cpp' -or -name '*.hpp' -or -name '*.h' \) -not -path "./src/json.hpp" -exec clang-format -style=file {} \;