//
//  main.cpp
//  Module 5
//
//  Created by Luke Contreras on 3/23/25.
//

// Module05ZookeeperChallenge.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

// =========================
// Section 1: Animal Classes
// =========================

// Base Animal class with enclosureID added
class Animal {
protected:
    string name;
    int age;
    string species;
    int enclosureID;  // New: Tracks which enclosure the animal is in
public:
    Animal(const string& name, int age, const string& species)
        : name(name), age(age), species(species), enclosureID(-1) {}
    virtual ~Animal() {}

    // Getters
    string getName() const { return name; }
    int getAge() const { return age; }
    string getSpecies() const { return species; }
    int getEnclosureID() const { return enclosureID; }

    // Setter for enclosure
    void setEnclosureID(int id) { enclosureID = id; }

    // Virtual method for unique info
    virtual string getUniqueInfo() const { return ""; }
};

// Hyena subclass
class Hyena : public Animal {
public:
    Hyena(const string& name, int age)
        : Animal(name, age, "Hyena") {}
    string getUniqueInfo() const override {
        return "Distinctive laugh and scavenger habits.";
    }
};

// Lion subclass with maneLength
class Lion : public Animal {
private:
    double maneLength; // in centimeters
public:
    Lion(const string& name, int age, double maneLength = 0.0)
        : Animal(name, age, "Lion"), maneLength(maneLength) {}
    string getUniqueInfo() const override {
        return "Mane length: " + to_string(maneLength) + " cm.";
    }
};

// Tiger subclass with stripeCount
class Tiger : public Animal {
private:
    int stripeCount;
public:
    Tiger(const string& name, int age, int stripeCount = 0)
        : Animal(name, age, "Tiger"), stripeCount(stripeCount) {}
    string getUniqueInfo() const override {
        return "Stripe count: " + to_string(stripeCount) + ".";
    }
};

// Bear subclass with hibernationDays
class Bear : public Animal {
private:
    int hibernationDays;
public:
    Bear(const string& name, int age, int hibernationDays = 0)
        : Animal(name, age, "Bear"), hibernationDays(hibernationDays) {}
    string getUniqueInfo() const override {
        return "Hibernation days: " + to_string(hibernationDays) + ".";
    }
};

// =========================
// Section 2: Main Function
// =========================
int main() {
    // 2.1: Vector to store all Animal objects
    vector<Animal*> animals;

    // -------------------------
    // Read arrivingAnimals.txt
    // -------------------------
    ifstream inputFile("arrivingAnimals.txt");
    if (!inputFile) {
        cerr << "Error: Unable to open arrivingAnimals.txt" << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        if (line.empty()) continue;  // skip empty lines
        istringstream iss(line);
        string species, name;
        int age;
        iss >> species >> name >> age;

        // Create the appropriate animal
        if (species == "Lion") {
            double maneLength;
            if (!(iss >> maneLength)) maneLength = 0.0;
            animals.push_back(new Lion(name, age, maneLength));
        } else if (species == "Tiger") {
            int stripeCount;
            if (!(iss >> stripeCount)) stripeCount = 0;
            animals.push_back(new Tiger(name, age, stripeCount));
        } else if (species == "Bear") {
            int hibernationDays;
            if (!(iss >> hibernationDays)) hibernationDays = 0;
            animals.push_back(new Bear(name, age, hibernationDays));
        } else if (species == "Hyena") {
            animals.push_back(new Hyena(name, age));
        } else {
            // Unknown species fallback
            Animal* generic = new Animal(name, age, species);
            animals.push_back(generic);
        }
    }
    inputFile.close();

    // -------------------------------------------------
    // 2.2: Read animalEnclosures.txt (Name -> Enclosure)
    // -------------------------------------------------
    ifstream enclosureFile("animalEnclosures.txt");
    map<string, int> enclosureAssignments; // <AnimalName, EnclosureID>
    if (enclosureFile) {
        while (getline(enclosureFile, line)) {
            if (line.empty()) continue;
            istringstream iss(line);
            string animalName;
            int encID;
            iss >> animalName >> encID;
            enclosureAssignments[animalName] = encID;
        }
        enclosureFile.close();
    } else {
        cerr << "Warning: Could not open animalEnclosures.txt. Proceeding without enclosure data." << endl;
    }

    // ----------------------------------------------
    // 2.3: Match each Animal with its enclosure (if any)
    // ----------------------------------------------
    for (Animal* animal : animals) {
        auto it = enclosureAssignments.find(animal->getName());
        if (it != enclosureAssignments.end()) {
            animal->setEnclosureID(it->second);
        }
    }

    // ----------------------------------------------
    // 2.4: Categorize animals by species
    // ----------------------------------------------
    map<string, vector<Animal*>> speciesMap;
    for (Animal* animal : animals) {
        speciesMap[animal->getSpecies()].push_back(animal);
    }

    // (Optional) Also categorize animals by enclosure:
    // map<int, vector<Animal*>> enclosureMap;
    // for (Animal* animal : animals) {
    //     enclosureMap[animal->getEnclosureID()].push_back(animal);
    // }

    // ----------------------------------------------
    // 2.5: Write the final report to zooReport.txt
    // ----------------------------------------------
    ofstream outputFile("zooReport.txt");
    if (!outputFile) {
        cerr << "Error: Unable to open zooReport.txt for writing." << endl;
        return 1;
    }

    for (const auto& entry : speciesMap) {
        const string& species = entry.first;
        const vector<Animal*>& listOfAnimals = entry.second;

        outputFile << "Species: " << species << "\n";
        for (Animal* a : listOfAnimals) {
            outputFile << "  Name: " << a->getName()
                       << ", Age: " << a->getAge()
                       << ", Enclosure: " << (a->getEnclosureID() == -1 ? "None" : to_string(a->getEnclosureID()))
                       << ", Unique Info: " << a->getUniqueInfo() << "\n";
        }
        outputFile << "Total count: " << listOfAnimals.size() << "\n\n";
    }
    outputFile.close();

    cout << "Report successfully generated in zooReport.txt" << endl;

    // ----------------------------------------------
    // 2.6: Clean up dynamic memory
    // ----------------------------------------------
    for (Animal* animal : animals) {
        delete animal;
    }

    return 0;
}
