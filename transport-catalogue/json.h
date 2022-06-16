#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <cstring> 
#include <unordered_map>
#include <sstream>

namespace json {

    class Node;
    // Сохраните объявления Dict и Array без изменения
    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;
    // Эта ошибка должна выбрасываться при ошибках парсинга JSON
    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class Node {
    public:
        /* Реализуйте Node, используя std::variant */
        Node() :
            json_node_(nullptr)
        {
        }
       template <typename T>
       Node(T value) :
           json_node_(std::move(value))
       {
       }
       bool IsInt() const;
       bool IsDouble() const;
       bool IsPureDouble() const;
       bool IsBool () const;
       bool IsString() const;
       bool IsNull() const;
       bool IsArray() const;
       bool IsMap() const;

       const Array& AsArray() const;
       const Dict& AsMap() const;
       const std::string& AsString() const;
       bool AsBool() const;
       int AsInt() const;
       double AsDouble() const;
       const std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>& GetValue() const;
     
       bool operator==(const Node& rhs) const;
       bool operator!=(const Node& rhs) const;

    private:
        std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string> json_node_;
    };

    class Document {
    public:
        explicit Document(Node root);

        const Node& GetRoot() const;

        bool operator ==(const Document& rhs) const;
        bool operator !=(const Document& rhs) const;

    private:
        Node root_;
    };
    static std::unordered_map<char, std::string> esc_symbols_save = {
    {'\\', std::string("\\\\")},
    {'"', std::string("\\\"")},
    {'\n', std::string("\\n")},
    {'\r',std::string("\\r")},
    {'\b',std::string("\\b")},
    {'\f',std::string("\\f")}
    };

    Document Load(std::istream& input);

    void Print(const Document& doc, std::ostream& output);
    struct OstreamSolutionPrinter
    {
        std::ostream& out;

        void operator()(std::nullptr_t) const
        {
            using namespace std::literals;
            out << "null"sv;  
        }
        void operator()(Array array) const
        {
            using namespace std::literals;
            out << "["sv << std::endl;
            bool print_comma = false;
            for (const auto& elem : array)
            {
                if (print_comma)
                {
                    out << ", "sv << std::endl;
                }
                std::visit(OstreamSolutionPrinter{ out }, elem.GetValue());
                print_comma = true;
            }
            out << std::endl << "]"sv;
        }
        void operator()(Dict map) const
        {
            using namespace std::literals;
            out << "{"sv << std::endl;
            bool print_comma = false;
            for (const auto& elem : map)
            {
                if (print_comma)
                {
                    out << ", "sv << std::endl;
                }
                out << "\""sv << elem.first << "\": "sv;
                std::visit(OstreamSolutionPrinter{ out }, elem.second.GetValue());
                print_comma = true;
            }
            out << std::endl << "}"sv;
        }
        void operator()(bool value) const
        {
            using namespace std::literals;
            if (value)
            {
                out << "true"sv; 
            }
            else
            {
                out << "false"sv;  
            }
        }
        void operator()(int num) const
        {
            out << num; 
        }
        void operator()(double num) const
        {
            out << num;  
        }
        void operator()(std::string line) const
        {
            using namespace std::literals;
            std::istringstream strm(line);
            out << "\""sv;
            char ch;
            while (strm.get(ch))
            {
                if (esc_symbols_save.find(ch) != esc_symbols_save.end())
                {
                    out << esc_symbols_save[ch];
                }
                else
                {
                    out << ch;
                }
            }
            out << "\""sv;
        }
    };
}  // namespace json