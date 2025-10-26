/* Base thingies  */
export enum Breakthrough{
    ADVANCED_DRONE_DRIVE = "Advanced Drone Drive",
    ALIEN_IMPRINTS = "Alien Imprints",
    ANCIENT_TERRAFORMING_DEVICE = "Ancient Terraforming Device",
    ARTIFICIAL_MUSCLES = "Artificial Muscles",
    AUTONOMOUS_HUBS = "Autonomous Hubs",
    CAPTURE_ASTEROID = "Capture Asteroid",
    CARGO_BAY_OF_HOLDING = "Cargobay of holding",
    CLONING = "Cloning",
    CONSTRUCTION_NANITES = "Construction Nanites",
    CORE_METALS = "Core Metals",
    CORE_RARE_METALS = "Core Rare Metals",
    CORE_WATER = "Core Water",
    CRYO_SLEEP = "Cryo-sleep",
    DESIGNED_FORESTATION = "Designed Forestation",
    DOME_STREAMLINING = "Dome Streamlining",
    DRY_FARMING = "Dry Farming",
    ETERNAL_FUSION = "Eternal Fusion",
    EXTRACTOR_AI = "Extractor AI",
    FACTORY_AUTOMATION = "Factory Automation",
    FOREVER_YOUNG = "Forever Young",
    FRICTIONLESS_COMPOSITES = "Frictionless Composites",
    GEM_ARCHITECTURE = "Gem Architecture",
    GENE_SELECTION = "Gene Selection",
    GIANT_CROPS = "Giant Crops",
    GLOBAL_SUPPORT = "Global Support",
    GOOD_VIBRATIONS = "Good Vibrations",
    HIVE_MIND = "Hive Mind",
    HULL_POLARIZATION = "Hull Polarization",
    HYPERSENSITIVE_PHOTOVOLTAICS = "Hypersensitive Photovoltaics",
    INSPIRING_ARCHITECTURE = "Inspiring Architecture",
    INTERPLANETARY_LEARNING = "Interplanetary Learning",
    LAKE_VAPORATORS = "Lake Vaporators",
    LANDSCAPING_NANITES = "Landscaping Nanites",
    MAGNETIC_EXTRACTION = "Magnetic Extraction",
    MARS_MARKETING = "Mars Marketing",
    MARTIAN_DIET = "Martian Diet",
    MARTIAN_STEEL = "Martian Steel",
    MARTIANBORN_INGENUITY = "Martianborn Ingenuity",
    MOLE_PEOPLE = "Mole People",
    MULTISPIRAL_ARCHITECTURE = "Multispiral Architecture",
    NANO_REFINEMENT = "Nano Refinement",
    NEOCONCRETE = "Neo-Concrete",
    NEURAL_EMPATHY = "Neural Empathy",
    NOCTURNAL_ADAPTATION = "Nocturnal Adaptation",
    OVERCHARGE_AMPLIFICATIONS = "Overcharge Amplifications",
    PLASMA_ROCKET = "Plasma Rocket",
    PLUTONIUM_SYNTHESIS = "Plutonium Synthesis",
    PREFAB_COMPRESSION = "Prefab Compression",
    PRINTED_ELECTRONICS = "Printed Electronics",
    PROJECT_PHOENIX = "Project Phoenix",
    RAPID_SLEEP = "Rapid Sleep",
    RESILIENT_VEGETATION = "Resilient Vegetation",
    SAFE_MODE = "Safe Mode",
    SERVICE_BOTS = "Service Bots",
    SOYLENT_GREEN = "Soylent Green",
    SPACE_REHABILITATION = "Space Rehabilitation",
    SUPERCONDUCTING_COMPUTING = "Superconducting Computing",
    SUPERFUNGUS = "Superfungus",
    SUPERIOR_CABLES = "Superior Cables",
    SUPERIOR_PIPES = "Superior Pipes",
    SUSTAINED_WORKLOAD = "Sustained Workload",
    THE_POSITRONIC_BRAIN = "The Positronic Brain",
    VECTOR_PUMP = "Vector Pump",
    VEHICLE_WEIGHT_OPTIMISATIONS = "Vehicle Weight Optimizations",
    VOCATION_ORIENTED_SOCIETY = "Vocation-Oriented Society",
    WIRELESS_POWER = "Wireless Power",
    ZERO_SPACE_COMPUTING = "Zero-Space Computing"
};

export const BreakthroughList = Object.values(Breakthrough);

export type BreakthroughSystem = {
    breakthrough: Breakthrough;
    system: string;
}

export const listBreakthroughSYSTEM: string[] = [
    "ADVANCED_DRONE_DRIVE",
    "ALIEN_IMPRINTS",
    "ANCIENT_TERRAFORMING_DEVICE",
    "ARTIFICIAL_MUSCLES",
    "AUTONOMOUS_HUBS",
    "CAPTURE_ASTEROID",
    "CARGO_BAY_OF_HOLDING",
    "CLONING",
    "CONSTRUCTION_NANITES",
    "CORE_METALS",
    "CORE_RARE_METALS",
    "CORE_WATER",
    "CRYO_SLEEP",
    "DESIGNED_FORESTATION",
    "DOME_STREAMLINING",
    "DRY_FARMING",
    "ETERNAL_FUSION",
    "EXTRACTOR_AI",
    "FACTORY_AUTOMATION",
    "FOREVER_YOUNG",
    "FRICTIONLESS_COMPOSITES",
    "GEM_ARCHITECTURE",
    "GENE_SELECTION",
    "GIANT_CROPS",
    "GLOBAL_SUPPORT",
    "GOOD_VIBRATIONS",
    "HIVE_MIND",
    "HULL_POLARIZATION",
    "HYPERSENSITIVE_PHOTOVOLTAICS",
    "INSPIRING_ARCHITECTURE",
    "INTERPLANETARY_LEARNING",
    "LAKE_VAPORATORS",
    "LANDSCAPING_NANITES",
    "MAGNETIC_EXTRACTION",
    "MARS_MARKETING",
    "MARTIAN_DIET",
    "MARTIAN_STEEL",
    "MARTIANBORN_INGENUITY",
    "MOLE_PEOPLE",
    "MULTISPIRAL_ARCHITECTURE",
    "NANO_REFINEMENT",
    "NEOCONCRETE",
    "NEURAL_EMPATHY",
    "NOCTURNAL_ADAPTATION",
    "OVERCHARGE_AMPLIFICATIONS",
    "PLASMA_ROCKET",
    "PLUTONIUM_SYNTHESIS",
    "PREFAB_COMPRESSION",
    "PRINTED_ELECTRONICS",
    "PROJECT_PHOENIX",
    "RAPID_SLEEP",
    "RESILIENT_VEGETATION",
    "SAFE_MODE",
    "SERVICE_BOTS",
    "SOYLENT_GREEN",
    "SPACE_REHABILITATION",
    "SUPERCONDUCTING_COMPUTING",
    "SUPERFUNGUS",
    "SUPERIOR_CABLES",
    "SUPERIOR_PIPES",
    "SUSTAINED_WORKLOAD",
    "THE_POSITRONIC_BRAIN",
    "VECTOR_PUMP",
    "VEHICLE_WEIGHT_OPTIMISATIONS",
    "VOCATION_ORIENTED_SOCIETY",
    "WIRELESS_POWER",
    "ZERO_SPACE_COMPUTING"
] as const;

export enum VariantEnum {
    STANDARD = "Standard",
    GREEN_PLANET = "Green Planet",
    BELOW_BEYOND = "Below and Beyond",
    BEYOND_GREEN = "Beyond + Green",
    TITO_GREEN_PLANET = "Tito: Green Planet",
    EVANS_GREEN_PLANET = "Evans: Green Planet"
};

export type VariantSystem = {
    variant: VariantEnum;
    system: string;
}

export const listVariantSYSTEM: VariantSystem[] = [
    {variant: VariantEnum.STANDARD, system: "STANDARD"},
    {variant: VariantEnum.GREEN_PLANET, system: "GREEN_PLANET"},
    {variant: VariantEnum.BELOW_BEYOND, system: "BELOW_BEYOND"},
    {variant: VariantEnum.BEYOND_GREEN, system: "BEYOND_GREEN"},
    {variant: VariantEnum.TITO_GREEN_PLANET, system: "TITO_GREEN_PLANET"},
    {variant: VariantEnum.EVANS_GREEN_PLANET, system: "EVANS_GREEN_PLANET"},
] as const;

export type BreakthroughVariant = {
    breakthrough: Breakthrough;
    variant: VariantEnum;
}

export const listBreakthroughVariant: BreakthroughVariant[] = [
    {breakthrough: Breakthrough.ADVANCED_DRONE_DRIVE, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.ALIEN_IMPRINTS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.ANCIENT_TERRAFORMING_DEVICE, variant: VariantEnum.GREEN_PLANET},
    {breakthrough: Breakthrough.ARTIFICIAL_MUSCLES, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.AUTONOMOUS_HUBS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.CAPTURE_ASTEROID, variant: VariantEnum.BELOW_BEYOND},
    {breakthrough: Breakthrough.CARGO_BAY_OF_HOLDING, variant: VariantEnum.BELOW_BEYOND},
    {breakthrough: Breakthrough.CLONING, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.CONSTRUCTION_NANITES, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.CORE_METALS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.CORE_RARE_METALS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.CORE_WATER, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.CRYO_SLEEP, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.DESIGNED_FORESTATION, variant: VariantEnum.GREEN_PLANET},
    {breakthrough: Breakthrough.DOME_STREAMLINING, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.DRY_FARMING, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.ETERNAL_FUSION, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.EXTRACTOR_AI, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.FACTORY_AUTOMATION, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.FOREVER_YOUNG, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.FRICTIONLESS_COMPOSITES, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.GEM_ARCHITECTURE, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.GENE_SELECTION, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.GIANT_CROPS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.GLOBAL_SUPPORT, variant: VariantEnum.BELOW_BEYOND},
    {breakthrough: Breakthrough.GOOD_VIBRATIONS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.HIVE_MIND, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.HULL_POLARIZATION, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.HYPERSENSITIVE_PHOTOVOLTAICS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.INSPIRING_ARCHITECTURE, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.INTERPLANETARY_LEARNING, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.LAKE_VAPORATORS, variant: VariantEnum.GREEN_PLANET},
    {breakthrough: Breakthrough.LANDSCAPING_NANITES, variant: VariantEnum.GREEN_PLANET},
    {breakthrough: Breakthrough.MAGNETIC_EXTRACTION, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.MARS_MARKETING, variant: VariantEnum.BELOW_BEYOND},
    {breakthrough: Breakthrough.MARTIAN_DIET, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.MARTIAN_STEEL, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.MARTIANBORN_INGENUITY, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.MOLE_PEOPLE, variant: VariantEnum.BELOW_BEYOND},
    {breakthrough: Breakthrough.MULTISPIRAL_ARCHITECTURE, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.NANO_REFINEMENT, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.NEOCONCRETE, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.NEURAL_EMPATHY, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.NOCTURNAL_ADAPTATION, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.OVERCHARGE_AMPLIFICATIONS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.PLASMA_ROCKET, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.PLUTONIUM_SYNTHESIS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.PREFAB_COMPRESSION, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.PRINTED_ELECTRONICS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.PROJECT_PHOENIX, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.RAPID_SLEEP, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.RESILIENT_VEGETATION, variant: VariantEnum.GREEN_PLANET},
    {breakthrough: Breakthrough.SAFE_MODE, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.SERVICE_BOTS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.SOYLENT_GREEN, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.SPACE_REHABILITATION, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.SUPERCONDUCTING_COMPUTING, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.SUPERFUNGUS, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.SUPERIOR_CABLES, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.SUPERIOR_PIPES, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.SUSTAINED_WORKLOAD, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.THE_POSITRONIC_BRAIN, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.VECTOR_PUMP, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.VEHICLE_WEIGHT_OPTIMISATIONS, variant: VariantEnum.BELOW_BEYOND},
    {breakthrough: Breakthrough.VOCATION_ORIENTED_SOCIETY, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.WIRELESS_POWER, variant: VariantEnum.STANDARD},
    {breakthrough: Breakthrough.ZERO_SPACE_COMPUTING, variant: VariantEnum.STANDARD}
] as const;

export type NamedLandingSystem = {
    system: string;
    locationName: string;
}

export const listNamedLanding: NamedLandingSystem[] = [
    {system: "ALBOR_THOLUS", locationName: "Albor Tholus"},
    {system: "ARABIA_TERRA", locationName: "Arabia Terra"},
    {system: "ARCADIA_PLANITIA", locationName: "Arcadia Planitia"},
    {system: "ARGYRE_PLANITIA", locationName: "Argyre Planitia"},
    {system: "ARSAI_MONS", locationName: "Arsai Mons"},
    {system: "ASCRAEUS_MONS", locationName: "Ascraeus Mons"},
    {system: "COLONY_SITE", locationName: "Colony Site"},
    {system: "ELYSIUM_MONS", locationName: "Elysium Mons"},
    {system: "GALE_CRATER", locationName: "Gale Crater"},
    {system: "HECATUS_THOLUS", locationName: "Hecatus Tholus"},
    {system: "HELLAS_PLANITIA", locationName: "Hellas Planitia"},
    {system: "MARS_PATHFINDER_LANDING_AREA", locationName: "Mars Pathfinder Landing Area"},
    {system: "NORTH_CAP", locationName: "North Cap"},
    {system: "OLYMPUS_MONS", locationName: "Olympus Mons"},
    {system: "OPPORTUNITY_LANDING_AREA", locationName: "Opportunity Landing Area"},
    {system: "PAVONIS_MONS", locationName: "Pavonis Mons"},
    {system: "SOUTH_CAP", locationName: "South Cap"},
    {system: "SPIRIT_LANDING_AREA", locationName: "Spirit Landing Area"},
    {system: "TEMPA_TERRA", locationName: "Tempa Terra"},
    {system: "TERRA_SABAEA", locationName: "Terra Sabaea"},
    {system: "TERRA_SIREUM", locationName: "Terra Sireum"},
    {system: "UTOPIA_PLANITIA", locationName: "Utopia Planitia"},
    {system: "VALLES_MARINERIS", locationName: "Valles Marineris"},
    {system: "VIKING_1_LANDING_AREA", locationName: "Viking 1 Landing Area"},
    {system: "VIKING_2_LANDING_AREA", locationName: "Viking 2 Landing Area"},
    {system: "UNNAMED", locationName: "Unnamed"},
] as const;

export type TopographySystem = {
    system: string;
    topographyName: string;
}

export const listTopography: TopographySystem[] = [
    {system: "RELATIVELY_FLAT", topographyName: "Relatively Flat"},
    {system: "STEEP", topographyName: "Steep"},
    {system: "ROUGH", topographyName: "Rough"},
    {system: "MOUNTAINOUS", topographyName: "Mountainous"},
] as const;

export type MapNameSystem = {
    system: string;
    mapName: string;
}

export const listMapNames: MapNameSystem[] = [
    {system: "BlankBig_01", mapName: "Blank 1"},
    {system: "BlankBig_02", mapName: "Blank 2"},
    {system: "BlankBig_03", mapName: "Blank 3"},
    {system: "BlankBig_04", mapName: "Blank 4"},
    {system: "BlankBigCrater_01", mapName: "Crater 1"},
    {system: "BlankBigHeartCMix_03", mapName: "Heart 3"},
    {system: "BlankTerraceBig_05", mapName: "Terrace 5"},
    {system: "BlankBigCanyonCMix_01", mapName: "Canyon 1"},
    {system: "BlankBigCanyonCMix_02", mapName: "Canyon 2"},
    {system: "BlankBigCanyonCMix_03", mapName: "Canyon 3"},
    {system: "BlankBigCanyonCMix_04", mapName: "Canyon 4"},
    {system: "BlankBigCanyonCMix_05", mapName: "Canyon 5"},
    {system: "BlankBigCanyonCMix_06", mapName: "Canyon 6"},
    {system: "BlankBigCanyonCMix_07", mapName: "Canyon 7"},
    {system: "BlankBigCanyonCMix_08", mapName: "Canyon 8"},
    {system: "BlankBigCanyonCMix_09", mapName: "Canyon 9"},
    {system: "BlankBigCliffsCMix_01", mapName: "Cliffs 1"},
    {system: "BlankBigCliffsCMix_02", mapName: "Cliffs 2"},
    {system: "BlankBigCratersCMix_01", mapName: "Crater Mix 1"},
    {system: "BlankBigTerraceCMix_01", mapName: "Terrace Mix 1"},
    {system: "BlankBigTerraceCMix_02", mapName: "Terrace Mix 2"},
    {system: "BlankBigTerraceCMix_03", mapName: "Terrace Mix 3"},
    {system: "BlankBigTerraceCMix_04", mapName: "Terrace Mix 4"},
    {system: "BlankBigTerraceCMix_05", mapName: "Terrace Mix 5"},
    {system: "BlankBigTerraceCMix_06", mapName: "Terrace Mix 6"},
    {system: "BlankBigTerraceCMix_07", mapName: "Terrace Mix 7"},
    {system: "BlankBigTerraceCMix_08", mapName: "Terrace Mix 8"},
    {system: "BlankBigTerraceCMix_09", mapName: "Terrace Mix 9"},
    {system: "BlankBigTerraceCMix_10", mapName: "Terrace Mix 10"},
    {system: "BlankBigTerraceCMix_11", mapName: "Terrace Mix 11"},
    {system: "BlankBigTerraceCMix_12", mapName: "Terrace Mix 12"},
    {system: "BlankBigTerraceCMix_13", mapName: "Terrace Mix 13"},
    {system: "BlankBigTerraceCMix_14", mapName: "Terrace Mix 14"},
    {system: "BlankBigTerraceCMix_15", mapName: "Terrace Mix 15"},
    {system: "BlankBigTerraceCMix_16", mapName: "Terrace Mix 16"},
    {system: "BlankBigTerraceCMix_17", mapName: "Terrace Mix 17"},
    {system: "BlankBigTerraceCMix_18", mapName: "Terrace Mix 18"},
    {system: "BlankBigTerraceCMix_19", mapName: "Terrace Mix 19"},
    {system: "BlankBigTerraceCMix_20", mapName: "Terrace Mix 20"},
] as const;


export enum ButtonState {
    IDLE = "btn-light",
    SEARCHING = "btn-dark",
    SUCCESS = "btn-success",
    FAILED = "btn-danger",
    CHANGED = "btn-warning"
}

export enum ComparitorState {
    LessThan = "Less/Equal:",
    EquealTo = "Equal:",
    MoreThan = "More/Equal:"
}

/* Simple Requests */
export interface BreakthroughRequest {
    all: string[];
}

export interface ValueRequest {
    Number: number;
    comparitor: ComparitorState; // True less than number, False more than number
}

export interface SimpleRequest {
    Variant?: string;
    Resources?: ValueRequest;
    Disasters?: ValueRequest;
    BreakthroughFilters?: BreakthroughRequest;
}

/* Complex Requests */
export interface ResourcesRequest {
    Water?: ValueRequest;
    Concrete?: ValueRequest;
    Metal?: ValueRequest;
    RareMetal?: ValueRequest;
}

export interface DisastersRequest {
    Meteors?: ValueRequest;
    ColdWaves?: ValueRequest;
    DustStorms?: ValueRequest;
    DustDevils?: ValueRequest;
}

export interface MapRequest {
    NamedArea?: string[];
    Topography?: string[];
    MapName?: string[];
    Challenge?: ValueRequest;
}

export interface ComplexRequest {
    Resources?: ResourcesRequest;
    Disasters?: DisastersRequest;
    Map?: MapRequest;
}

/* Sorting Requests */
export interface SortingRequest {
    isLowest: boolean;
    SortItem: string;
}

/* Page Specific */
export interface PageSite {
    ID: number;
    Coordinates: string;
    DisastersTot: number;
    ResourcesTot: number;

    DustDevils: number;
    DustStorms: number;
    Meteors: number;
    ColdWaves: number;

    Concrete: number;
    Water: number;
    Metal: number;
    RareMetal: number;
}

/* COORDINATES */
export interface CoordinateObj {
    NSNum: number;
    NSChar: "N" | "S";
    EWNum: number;
    EWChar: "E" | "W";
}

/* Comms */
export interface PageRequest {
    Simple: SimpleRequest;
    Complex?: ComplexRequest;
    Sorting?: SortingRequest;
    PageNum?: number;
}

export interface PageResponse {
    VariantSystem: string;
    PageNum: number;
    PageTot: number;
    PageSites: PageSite[];
    Response?: string;
}

export interface SiteRequest {
    VariantSystem: string;
    SiteID: number;
}
export interface SiteRequest {
    VariantSystem: string;
    SiteID: number;
}

export interface SiteResponse {
    Response?: string;
    VariantSystem: string;
    SiteID: number;

    Coordinate: CoordinateObj;

    NamedLandingSYS: string;
    TopographySYS: string;
    MapName: string;

    Altitude: number;
    Temperature: number;
    Challenge: number;

    DustDevils: number;
    DustStorms: number;
    Meteors: number;
    ColdWaves: number;

    Concrete: number;
    Water: number;
    Metal: number;
    RareMetal: number;

    MissingBreakthroughs: number;
    Breakthroughs: string[];
}

export interface CoordRequest {
    VariantSystem: string;
    Coord: CoordinateObj;
}