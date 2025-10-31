#pragma once

#include <array>
#include <map>
#include <string>
#include <bitset>

namespace Breakthroughs {

    static const enum breakthrough_Enum: uint8_t {
        ADVANCED_DRONE_DRIVE = 0,
        ALIEN_IMPRINTS,
        ANCIENT_TERRAFORMING_DEVICE,
        ARTIFICIAL_MUSCLES,
        AUTONOMOUS_HUBS,
        CAPTURE_ASTEROID,
        CARGO_BAY_OF_HOLDING,
        CLONING,
        CONSTRUCTION_NANITES,
        CORE_METALS,
        CORE_RARE_METALS,
        CORE_WATER,
        CRYO_SLEEP,
        DESIGNED_FORESTATION,
        DOME_STREAMLINING,
        DRY_FARMING,
        ETERNAL_FUSION,
        EXTRACTOR_AI,
        FACTORY_AUTOMATION,
        FOREVER_YOUNG,
        FRICTIONLESS_COMPOSITES,
        GEM_ARCHITECTURE,
        GENE_SELECTION,
        GIANT_CROPS,
        GLOBAL_SUPPORT,
        GOOD_VIBRATIONS,
        HIVE_MIND,
        HULL_POLARIZATION,
        HYPERSENSITIVE_PHOTOVOLTAICS,
        INSPIRING_ARCHITECTURE,
        INTERPLANETARY_LEARNING,
        LAKE_VAPORATORS,
        LANDSCAPING_NANITES,
        MAGNETIC_EXTRACTION,
        MARS_MARKETING,
        MARTIAN_DIET,
        MARTIAN_STEEL,
        MARTIANBORN_INGENUITY,
        MOLE_PEOPLE,
        MULTISPIRAL_ARCHITECTURE,
        NANO_REFINEMENT,
        NEOCONCRETE,
        NEURAL_EMPATHY,
        NOCTURNAL_ADAPTATION,
        OVERCHARGE_AMPLIFICATIONS,
        PLASMA_ROCKET,
        PLUTONIUM_SYNTHESIS,
        PREFAB_COMPRESSION,
        PRINTED_ELECTRONICS,
        PROJECT_PHOENIX,
        RAPID_SLEEP,
        RESILIENT_VEGETATION,
        SAFE_MODE,
        SERVICE_BOTS,
        SOYLENT_GREEN,
        SPACE_REHABILITATION,
        SUPERCONDUCTING_COMPUTING,
        SUPERFUNGUS,
        SUPERIOR_CABLES,
        SUPERIOR_PIPES,
        SUSTAINED_WORKLOAD,
        THE_POSITRONIC_BRAIN,
        VECTOR_PUMP,
        VEHICLE_WEIGHT_OPTIMISATIONS,
        VOCATION_ORIENTED_SOCIETY,
        WIRELESS_POWER,
        ZERO_SPACE_COMPUTING,
        END_NONE
    };

    static const std::map<std::string, breakthrough_Enum> stringToEnum {
        {"Advanced Drone Drive", breakthrough_Enum::ADVANCED_DRONE_DRIVE},
        {"Alien Imprints", breakthrough_Enum::ALIEN_IMPRINTS},
        {"Ancient Terraforming Device", breakthrough_Enum::ANCIENT_TERRAFORMING_DEVICE},
        {"Artificial Muscles", breakthrough_Enum::ARTIFICIAL_MUSCLES},
        {"Autonomous Hubs", breakthrough_Enum::AUTONOMOUS_HUBS},
        {"Capture Asteroid", breakthrough_Enum::CAPTURE_ASTEROID},
        {"Cargobay of holding", breakthrough_Enum::CARGO_BAY_OF_HOLDING},
        {"Cloning", breakthrough_Enum::CLONING},
        {"Construction Nanites", breakthrough_Enum::CONSTRUCTION_NANITES},
        {"Core Metals", breakthrough_Enum::CORE_METALS},
        {"Core Rare Metals", breakthrough_Enum::CORE_RARE_METALS},
        {"Core Water", breakthrough_Enum::CORE_WATER},
        {"Cryo-sleep", breakthrough_Enum::CRYO_SLEEP},
        {"Designed Forestation", breakthrough_Enum::DESIGNED_FORESTATION},
        {"Dome Streamlining", breakthrough_Enum::DOME_STREAMLINING},
        {"Dry Farming", breakthrough_Enum::DRY_FARMING},
        {"Eternal Fusion", breakthrough_Enum::ETERNAL_FUSION},
        {"Extractor AI", breakthrough_Enum::EXTRACTOR_AI},
        {"Factory Automation", breakthrough_Enum::FACTORY_AUTOMATION},
        {"Forever Young", breakthrough_Enum::FOREVER_YOUNG},
        {"Frictionless Composites", breakthrough_Enum::FRICTIONLESS_COMPOSITES},
        {"Gem Architecture", breakthrough_Enum::GEM_ARCHITECTURE},
        {"Gene Selection", breakthrough_Enum::GENE_SELECTION},
        {"Giant Crops", breakthrough_Enum::GIANT_CROPS},
        {"Global Support", breakthrough_Enum::GLOBAL_SUPPORT},
        {"Good Vibrations", breakthrough_Enum::GOOD_VIBRATIONS},
        {"Hive Mind", breakthrough_Enum::HIVE_MIND},
        {"Hull Polarization", breakthrough_Enum::HULL_POLARIZATION},
        {"Hypersensitive Photovoltaics", breakthrough_Enum::HYPERSENSITIVE_PHOTOVOLTAICS},
        {"Inspiring Architecture", breakthrough_Enum::INSPIRING_ARCHITECTURE},
        {"Interplanetary Learning", breakthrough_Enum::INTERPLANETARY_LEARNING},
        {"Lake Vaporators", breakthrough_Enum::LAKE_VAPORATORS},
        {"Landscaping Nanites", breakthrough_Enum::LANDSCAPING_NANITES},
        {"Magnetic Extraction", breakthrough_Enum::MAGNETIC_EXTRACTION},
        {"Mars Marketing", breakthrough_Enum::MARS_MARKETING},
        {"Martian Diet", breakthrough_Enum::MARTIAN_DIET},
        {"Martian Steel", breakthrough_Enum::MARTIAN_STEEL},
        {"Martianborn Ingenuity", breakthrough_Enum::MARTIANBORN_INGENUITY},
        {"Mole People", breakthrough_Enum::MOLE_PEOPLE},
        {"Multispiral Architecture", breakthrough_Enum::MULTISPIRAL_ARCHITECTURE},
        {"Nano Refinement", breakthrough_Enum::NANO_REFINEMENT},
        {"Neo-Concrete", breakthrough_Enum::NEOCONCRETE},
        {"Neural Empathy", breakthrough_Enum::NEURAL_EMPATHY},
        {"Nocturnal Adaptation", breakthrough_Enum::NOCTURNAL_ADAPTATION},
        {"Overcharge Amplifications", breakthrough_Enum::OVERCHARGE_AMPLIFICATIONS},
        {"Plasma Rocket", breakthrough_Enum::PLASMA_ROCKET},
        {"Plutonium Synthesis", breakthrough_Enum::PLUTONIUM_SYNTHESIS},
        {"Prefab Compression", breakthrough_Enum::PREFAB_COMPRESSION},
        {"Printed Electronics", breakthrough_Enum::PRINTED_ELECTRONICS},
        {"Project Phoenix", breakthrough_Enum::PROJECT_PHOENIX},
        {"Rapid Sleep", breakthrough_Enum::RAPID_SLEEP},
        {"Resilient Vegetation", breakthrough_Enum::RESILIENT_VEGETATION},
        {"Safe Mode", breakthrough_Enum::SAFE_MODE},
        {"Service Bots", breakthrough_Enum::SERVICE_BOTS},
        {"Soylent Green", breakthrough_Enum::SOYLENT_GREEN},
        {"Space Rehabilitation", breakthrough_Enum::SPACE_REHABILITATION},
        {"Superconducting Computing", breakthrough_Enum::SUPERCONDUCTING_COMPUTING},
        {"Superfungus", breakthrough_Enum::SUPERFUNGUS},
        {"Superior Cables", breakthrough_Enum::SUPERIOR_CABLES},
        {"Superior Pipes", breakthrough_Enum::SUPERIOR_PIPES},
        {"Sustained Workload", breakthrough_Enum::SUSTAINED_WORKLOAD},
        {"The Positronic Brain", breakthrough_Enum::THE_POSITRONIC_BRAIN},
        {"Vector Pump", breakthrough_Enum::VECTOR_PUMP},
        {"Vehicle Weight Optimizations", breakthrough_Enum::VEHICLE_WEIGHT_OPTIMISATIONS},
        {"Vocation-Oriented Society", breakthrough_Enum::VOCATION_ORIENTED_SOCIETY},
        {"Wireless Power", breakthrough_Enum::WIRELESS_POWER},
        {"Zero-Space Computing", breakthrough_Enum::ZERO_SPACE_COMPUTING}
    };

    struct btrData {
        std::bitset<Breakthroughs::breakthrough_Enum::END_NONE> bitset;

        static bool getBreakthroughSet(btrData* btr, Breakthroughs::breakthrough_Enum breakthough) {
            return btr->bitset.test(breakthough);
        }
        static bool setBreakthroughSet(btrData* btr, std::array<Breakthroughs::breakthrough_Enum, 13>* btrs) {
            for (auto bth : *btrs) {
                btr->bitset.flip(bth);
                if (!btr->bitset.test(bth)) {
                    return false;
                }
            }
            return true;
        }

        bool operator < (const btrData& otherData) const {
            return bitset.count() < otherData.bitset.count();
        }
    };
}
