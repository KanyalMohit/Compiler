#include <iostream>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "codegen/codegen.h"
#include "codegen/vm.h"

int main() {
    try {
        // Test input with for loop
        std::string input = R"(
            for (var i = 6; i >= 0; i = i - 1) {
                print(i);
            }
        )";

        std::cout << "Input program:\n" << input << "\n\n";

        // Lexing
        Lexer lexer(input);
        auto tokens = lexer.tokenize();
        std::cout << "Tokens:\n";
        for (const auto& token : tokens) {
            std::cout << "Type: " << static_cast<int>(token.type) << ", Value: \"" << token.value << "\"\n";
        }
        std::cout << "\n";

        // Parsing
        Parser parser(tokens);
        auto statements = parser.parse();
        std::cout << "AST:\n";
        for (const auto& stmt : statements) {
            stmt->print(std::cout);
            std::cout << "\n";
        }
        std::cout << "\n";

        // Code Generation
        CodeGenerator generator;
        auto block = std::make_unique<BlockStmt>(std::move(statements));
        auto program = generator.generate(block.get());
        
        std::cout << "Bytecode:\n";
        for (size_t i = 0; i < program.instructions.size(); i++) {
            const auto& instr = program.instructions[i];
            std::cout << i << ": ";
            switch (instr.op) {
                case OpCode::PUSH:
                    if (std::holds_alternative<int>(instr.operand)) {
                        std::cout << "PUSH " << std::get<int>(instr.operand);
                    } else if (std::holds_alternative<std::string>(instr.operand)) {
                        std::cout << "PUSH \"" << std::get<std::string>(instr.operand) << "\"";
                    }
                    break;
                case OpCode::POP:
                    std::cout << "POP";
                    break;
                case OpCode::STORE:
                    std::cout << "STORE " << std::get<int>(instr.operand);
                    break;
                case OpCode::LOAD:
                    std::cout << "LOAD " << std::get<int>(instr.operand);
                    break;
                case OpCode::ADD:
                    std::cout << "ADD";
                    break;
                case OpCode::SUB:
                    std::cout << "SUB";
                    break;
                case OpCode::MUL:
                    std::cout << "MUL";
                    break;
                case OpCode::DIV:
                    std::cout << "DIV";
                    break;
                case OpCode::CMP_LT:
                    std::cout << "CMP_LT";
                    break;
                case OpCode::CMP_LE:
                    std::cout << "CMP_LE";
                    break;
                case OpCode::CMP_GT:
                    std::cout << "CMP_GT";
                    break;
                case OpCode::CMP_GE:
                    std::cout << "CMP_GE";
                    break;
                case OpCode::JMP_IF_FALSE:
                    std::cout << "JMP_IF_FALSE " << std::get<int>(instr.operand);
                    break;
                case OpCode::JMP:
                    std::cout << "JMP " << std::get<int>(instr.operand);
                    break;
                case OpCode::PRINT:
                    std::cout << "PRINT";
                    break;
                case OpCode::HALT:
                    std::cout << "HALT";
                    break;
                default:
                    std::cout << "UNKNOWN";
            }
            std::cout << "\n";
        }
        std::cout << "\n";

        // Execution
        std::cout << "Output:\n";
        VirtualMachine vm;
        vm.execute(program);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
