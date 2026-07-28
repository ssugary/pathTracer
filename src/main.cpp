#include <iostream>
#include <chrono>
#include <string>

#include "API.hpp"

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        std::cerr << "Uso: " << argv[0] << " <caminho_para_o_arquivo_xml>\n";
        std::cerr << "Exemplo: " << argv[0] << " scenes/spotlights.xml\n";
        return EXIT_FAILURE;
    }

    std::string sceneFilePath = argv[1];

    std::cout << "========================================\n";
    std::cout << "                    SSRT                \n";
    std::cout << "========================================\n";
    std::cout << "[Main] Iniciando carregamento da cena: " << sceneFilePath << "\n\n";

    auto startTime = std::chrono::high_resolution_clock::now();

    ssrt::RunningOptions ro;
    try 
    {
        ssrt::Parser parser;
        parser.validate_arguments(argc, argv, ro);
        ssrt::API::initEngine(ro);
        parser.parse_scene(sceneFilePath);
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "\n[ERRO CRÍTICO] Execução interrompida: " << e.what() << "\n";
        return EXIT_FAILURE;
    } 
    catch (...) 
    {
        std::cerr << "\n[ERRO CRÍTICO] Ocorreu um erro desconhecido durante o parsing ou renderização.\n";
        return EXIT_FAILURE;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalTime = endTime - startTime;

    std::cout << "\n========================================\n";
    std::cout << "[Main] Processo concluído com sucesso em " 
              << totalTime.count() << " segundos.\n";
    std::cout << "========================================\n";
    std::cout << "\n========================================\n";
    std::cout << "Arquivo salvo em " << ro.outfile << "  \n";
    std::cout << "========================================\n";

    return EXIT_SUCCESS;

}