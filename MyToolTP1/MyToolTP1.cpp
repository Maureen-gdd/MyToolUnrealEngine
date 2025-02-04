// MyToolTP1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem>

#include "json.hpp"
using json = nlohmann::json;

void displayError(const std::string& message, int code = 1) {
	std::cerr << "Erreur: " << message << std::endl;
	exit(code);
}

int executeCommand(const std::string& command) {
	int result = std::system(command.c_str());
	if (result != 0) {
		std::cerr << "Erreur lors de l'exécution de la commande: " << command << std::endl;
	}
	return result;
}

json loadJsonFile(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		displayError("Impossible d'ouvrir le fichier " + path);
	}

	json data;
	try {
		file >> data;
	} catch (const std::exception& e) {
		displayError("Erreur lors de la lecture du fichier JSON: " + std::string(e.what()));
	}

	return data;
}

void showProjectInfo(const json& data) {
	std::string projectName = "Nom inconnu";
	
	if (data.contains("Modules")) {
		const auto& modules = data["Modules"];
		if (!modules.empty() && modules[0].contains("Name")) {
			projectName = modules[0].value("Name", "Nom inconnu");
		}
	}
	std::cout << "\n";
	std::cout << "Nom du projet : " << projectName << std::endl;
	
	std::regex versionRegex("^\\d+\\.\\d+$");
	std::string engineVersion = data.value("EngineAssociation", "Version inconnue");

	if (std::regex_match(engineVersion, versionRegex)) {
		std::cout << "Version d'Unreal Engine : " << engineVersion << std::endl;
	} else {
		std::cout << "Version d'Unreal Engine : From Source" << std::endl;
	}
	
	if (data.contains("Plugins")) {
		std::cout << "Plugins utilises :\n";
		for (const auto& plugin : data["Plugins"]) {
			std::string pluginName = plugin.value("Name", "Nom inconnu");
			bool isEnabled = plugin.value("Enabled", false);
			std::cout << "- " << pluginName;
			std::cout << (isEnabled ? " (Active)" : " (Desactive)") << std::endl;
		}
	} else {
		std::cout << "Aucun plugin utilisé.\n";
	}

	std::cout << "\n";
}

void buildProject(const std::string& path) {
	std::filesystem::path fsPath(path);
	std::string projectName = fsPath.stem().string();  // Récupère le nom du fichier sans extension
	
	// Assure-toi que le chemin du projet Unreal est bien passé entre guillemets
	std::string command = "powershell /C \"./Engine/Build/BatchFiles/Build.bat " +
						  projectName + " Win64 Development \"" +
						  fsPath.string() + "\" -waitmutex\"";
	
	if (executeCommand(command)) {
		std::cout << "Build terminé avec succès !" << std::endl;
	} else {
		std::cout << "Erreur lors du build." << std::endl;
	}
}

void packageProject(const std::string& path, const std::string& packagePath) {
	std::string command = "powershell /C ./Engine/Build/BatchFiles/RunUAT.bat -ScriptsForProject=" +
						  path + " BuildCookRun -project=" + path + " -noP4 -clientconfig=Shipping -serverconfig=Shipping -nocompileeditor -utf8output -platform=Win64 -build -cook -unversionedcookedcontent -stage -package -archive -archivedirectory=" + packagePath;

	if (executeCommand(command) == 0) {
		std::cout << "Packaging terminé avec succès !" << std::endl;
	} else {
		std::cout << "Erreur lors du packaging." << std::endl;
	}
}


int main(int argc, char* argv[])
{
	if (argc < 3) {
		displayError("Nombre d'arguments insuffisant. Usage: MyToolTP1 <chemin_fichier> <commande>\n");
	}

	std::string path = argv[1];
	std::string command = argv[2];
	std::string packagePath = (argc > 3) ? argv[3] : "";

	try {
		if (command == "show-infos") {
			json data = loadJsonFile(path);
			showProjectInfo(data);
		} else if (command == "build") {
			buildProject(path);
		} else if (command == "package") {
			packageProject(path, packagePath);
		} else {
			displayError("Commande non reconnue.");
		}
	} catch (const std::exception& e) {
		displayError(e.what());
	}
	
    return 0;
}
