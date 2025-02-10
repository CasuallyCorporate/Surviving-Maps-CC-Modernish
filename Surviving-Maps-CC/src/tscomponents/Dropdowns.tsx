import {MutableRefObject, useRef, useState} from "react";
import Multiselect from "multiselect-react-dropdown";
import { listBreakthroughVariant, listVariantSYSTEM, VariantSystem,
    BreakthroughVariant, SimpleRequest, ComplexRequest, Breakthrough,
    listNamedLanding, listTopography, listMapNames, NamedLandingSystem,
    TopographySystem, MapNameSystem} from "./SM-Constants";

interface LessMoreProps {
    isLess: Boolean;
    clickAction(): any;
}

interface DropdownProps {
    returnSimpleRef?: MutableRefObject<SimpleRequest>;
    returnComplexref?: MutableRefObject<ComplexRequest>;
    returnCoordinateRef?: MutableRefObject<string>;
    selectionChanged(): any;
    classNames?: string;
}

interface ResourceProps {
    resource: string;
}
interface DisasterProps {
    disaster: string;
}

function refError(str: string) {
    console.error("There was a ref error in dropdown: " + str);
}

/* Less/More Button */
export const LessMoreButton = (props: LessMoreProps ) => {

    if(props.isLess){
        return (
            <button onClick={props.clickAction} className="LessMoreButton"> <div className="LessMore"><p>Less</p><p className="strikedown">More</p></div><p id="than">or =</p> </button>
        );
    } else {
        return (
            <button onClick={props.clickAction} className="LessMoreButton"> <div className="LessMore"><p className="strikeup">Less</p><p>More</p></div><p id="than">or =</p> </button>
        );
    }
}

/* Simple Dropdowns */
export const DropGameVariant =  (props: DropdownProps) => {

    const firstRun = useRef(true);

    if(firstRun.current) {
        // Set the default variant
        if(props.returnSimpleRef) {
            props.returnSimpleRef.current.Variant = listVariantSYSTEM[0].system;
        } else {
            refError("DropGameVariant");
        }
        firstRun.current = false;
    }

    function selectionChanged(selectedList: VariantSystem[]) {
        if(props.returnSimpleRef) {
            props.returnSimpleRef.current.Variant = selectedList[0].system;
        } else {
            refError("DropGameVariant");
        }
        props.selectionChanged();
    }

    return (
        <>
        <div className={props.classNames}>
            <div id="WholeWidth">
            <Multiselect
                onKeyPressFn={function noRefCheck(){}}
                onRemove={function noRefCheck(){}}
                onSearch={function noRefCheck(){}}
                onSelect={selectionChanged}
                displayValue="variant"
                options={listVariantSYSTEM}
                singleSelect
                placeholder="Variant"
                selectedValues={[listVariantSYSTEM[0]]}
                customCloseIcon={<></>}
            />
            </div>
        </div>
        </>
    );
}

export const DropBreakthrough = (props: DropdownProps) => {


    function selectionChanged(selectedList: BreakthroughVariant[]) {
        let currList: string[] = [];

        // Create a Breakthrough list from the selectedList
        for (let i = 0; i < selectedList.length; i++) {
            currList.push(Object.keys(Breakthrough)[Object.values(Breakthrough).indexOf(selectedList[i].breakthrough)]);
        }
        
        if (props.returnSimpleRef) {
            // Overwrite/set the current list
            if(currList.length > 0) {
                props.returnSimpleRef.current.BreakthroughFilters = {all: currList};
            } else {
                props.returnSimpleRef.current.BreakthroughFilters = undefined;
            }
        } else {
            refError("DropBreakthrough");
        }
        props.selectionChanged();
    }

    return (
        <>
        <div className={props.classNames + " tool-tipped"}>
            <div id="WholeWidth">
            <Multiselect
                onKeyPressFn={function noRefCheck(){}}
                onRemove={selectionChanged}
                onSearch={function noRefCheck(){}}
                onSelect={selectionChanged}
                displayValue="breakthrough"
                groupBy="variant"
                options={listBreakthroughVariant}
                selectionLimit={13}
                placeholder="Breakthroughs"
            />
            </div>
            <div className="hover-tooltip">
                "Below and Beyond" searches only cover map anomaly based breakthroughs. "Below and Beyond" Planetary view breakthroughs are random.
            </div>
        </div>
        </>
    );
}

export const DropResources = (props: DropdownProps) => {

    const [isLessState, setIsLessState] = useState(false);

    function toggleLessMore() {
        setIsLessState(!isLessState);
    }

    function blankSearch() {
        if(props.returnSimpleRef) {
            props.returnSimpleRef.current.Resources = undefined;
        } else {
            refError("DropResources");
        }
        props.selectionChanged();
    }

    function selectionChanged(selectedList: number[]) {
        if(props.returnSimpleRef) {
            props.returnSimpleRef.current.Resources = { Number: selectedList[0], isLessThan: isLessState };
        } else {
            refError("DropResources");
        }
        props.selectionChanged();
    }

    return (
        <>
        <div className={props.classNames}>
            <LessMoreButton isLess={isLessState} clickAction={toggleLessMore} />
            <div id="WholeWidth">
            <Multiselect
                isObject={false}
                onKeyPressFn={function noRefCheck(){}}
                onRemove={blankSearch}
                onSearch={function noRefCheck(){}}
                onSelect={selectionChanged}
                options={[...Array(17).keys()].slice(4,17)}
                singleSelect
                placeholder="Resources"
                customCloseIcon={<>❌</>}
            />
            </div>
        </div>
        </>
    );
}

export const DropDisasters = (props: DropdownProps) => {
    const [isLessState, setIsLessState] = useState(false);

    function toggleLessMore() {
        setIsLessState(!isLessState);
    }

    function blankSearch() {
        if(props.returnSimpleRef) {
            props.returnSimpleRef.current.Disasters = undefined;
        } else {
            refError("DropDisasters");
        }
        props.selectionChanged();
    }

    function selectionChanged(selectedList: number[]) {
        if(props.returnSimpleRef) {
            props.returnSimpleRef.current.Disasters = { Number: selectedList[0], isLessThan: isLessState };
        } else {
            refError("DropDisasters");
        }
        props.selectionChanged();
    }

    return (
        <>
        <div className={props.classNames}>
            <LessMoreButton isLess={isLessState} clickAction={toggleLessMore} />
            <div id="WholeWidth">
            <Multiselect
                isObject={false}
                onKeyPressFn={function noRefCheck(){}}
                onRemove={blankSearch}
                onSearch={function noRefCheck(){}}
                onSelect={selectionChanged}
                options={[...Array(11).keys()].slice(4,11)}
                singleSelect
                placeholder="Disasters"
                customCloseIcon={<>❌</>}
            />
            </div>
        </div>
        </>
    );
}

/* Complex Dropdowns */

/* Generic RESOURCE Dropdown */
export const DropComResource = (props: DropdownProps & ResourceProps) => {
    const [isLessState, setIsLessState] = useState(false);

    function toggleLessMore() {
        setIsLessState(!isLessState);
        // Set isLessThan
        if(props.returnComplexref && props.returnComplexref.current.Resources) {
            switch(props.resource) {
                case "Water":
                    if(props.returnComplexref.current.Resources.Water) {
                        props.returnComplexref.current.Resources.Water = {
                            ...props.returnComplexref.current.Resources.Water,
                            isLessThan: !isLessState
                        }
                    }
                    break;
                case "Concrete":
                    if(props.returnComplexref.current.Resources.Concrete) {
                        props.returnComplexref.current.Resources.Concrete = {
                            ...props.returnComplexref.current.Resources.Concrete,
                            isLessThan: !isLessState
                        }
                    }
                    break;
                case "Metal":
                    if(props.returnComplexref.current.Resources.Metal) {
                        props.returnComplexref.current.Resources.Metal = {
                            ...props.returnComplexref.current.Resources.Metal,
                            isLessThan: !isLessState
                        }
                    }
                    break;
                case "Rare Metal":
                    if(props.returnComplexref.current.Resources.RareMetal) {
                        props.returnComplexref.current.Resources.RareMetal = {
                            ...props.returnComplexref.current.Resources.RareMetal,
                            isLessThan: !isLessState
                        }
                    }
                    break;
            }
        }
    }

    function selectionChanged(selectedList: number[]) {
        if(props.returnComplexref) {
            if(selectedList.length == 0){
                // Delete the current resource
                switch(props.resource) {
                    case "Water":
                        props.returnComplexref.current.Resources = {
                            ...props.returnComplexref.current.Resources,
                            Water: undefined
                        }
                        break;
                    case "Concrete":
                        props.returnComplexref.current.Resources = {
                            ...props.returnComplexref.current.Resources,
                            Concrete: undefined
                        }
                        break;
                    case "Metal":
                        props.returnComplexref.current.Resources = {
                            ...props.returnComplexref.current.Resources,
                            Metal: undefined
                        }
                        break;
                    case "Rare Metal":
                        props.returnComplexref.current.Resources = {
                            ...props.returnComplexref.current.Resources,
                            RareMetal: undefined
                        }
                        break;
                }

                // Check if there are any other complex resources. If not, delete the Resources 'node'
                if(props.returnComplexref.current.Resources) {
                    // using Object.keys() still returns a value above 0. Need to find a better way than this
                    if(JSON.stringify(props.returnComplexref.current.Resources) === "{}") {
                        props.returnComplexref.current.Resources = undefined;
                    }
                }
                props.selectionChanged();
                return;
            }
            switch(props.resource) {
                case "Water":
                    props.returnComplexref.current.Resources = {
                        ...props.returnComplexref.current.Resources,
                        Water: { Number: selectedList[0], isLessThan: isLessState }
                    }
                    break;
                case "Concrete":
                    props.returnComplexref.current.Resources = {
                        ...props.returnComplexref.current.Resources,
                        Concrete: { Number: selectedList[0], isLessThan: isLessState }
                    }
                    break;
                case "Metal":
                    props.returnComplexref.current.Resources = {
                        ...props.returnComplexref.current.Resources,
                        Metal: { Number: selectedList[0], isLessThan: isLessState }
                    }
                    break;
                case "Rare Metal":
                    props.returnComplexref.current.Resources = {
                        ...props.returnComplexref.current.Resources,
                        RareMetal: { Number: selectedList[0], isLessThan: isLessState }
                    }
                    break;
            }
        } else {
            refError("DropComResource");
        }
        props.selectionChanged();
    }

    return (
        <>
        <div className={props.classNames + " DropComplex"}>
            <h6>{props.resource}</h6>
            <div id="Dropdown">
                <LessMoreButton isLess={isLessState} clickAction={toggleLessMore} />
                <Multiselect
                isObject={false}
                onKeyPressFn={function noRefCheck(){}}
                onRemove={selectionChanged}
                onSearch={function noRefCheck(){}}
                onSelect={selectionChanged}
                options={[...Array(5).keys()].slice(1,5)}
                singleSelect
                placeholder={props.resource}
                customCloseIcon={<>❌</>}
                />
            </div>
        </div>
        </>
    );
}
/* Generic DISASTER Dropdown */
export const DropComDisaster = (props: DropdownProps & DisasterProps) => {
    const [isLessState, setIsLessState] = useState(false);

    function toggleLessMore() {
        setIsLessState(!isLessState);
        // Set the new isLessThan
        if(props.returnComplexref && props.returnComplexref.current.Disasters) {
            switch(props.disaster) {
                case "Meteors":
                    if(props.returnComplexref.current.Disasters.Meteors) {
                        props.returnComplexref.current.Disasters.Meteors = {
                            ...props.returnComplexref.current.Disasters.Meteors,
                            isLessThan: !isLessState
                        }
                    }
                    break;
                case "Cold Waves":
                    if(props.returnComplexref.current.Disasters.ColdWaves) {
                        props.returnComplexref.current.Disasters.ColdWaves = {
                            ...props.returnComplexref.current.Disasters.ColdWaves,
                            isLessThan: !isLessState
                        }
                    }
                    break;
                case "Dust Storms":
                    if(props.returnComplexref.current.Disasters.DustStorms) {
                        props.returnComplexref.current.Disasters.DustStorms = {
                            ...props.returnComplexref.current.Disasters.DustStorms,
                            isLessThan: !isLessState
                        }
                    }
                    break;
                case "Dust Devils":
                    if(props.returnComplexref.current.Disasters.DustDevils) {
                        props.returnComplexref.current.Disasters.DustDevils = {
                            ...props.returnComplexref.current.Disasters.DustDevils,
                            isLessThan: !isLessState
                        }
                    }
                    break;
            }
        } else { refError("DropComResource:toggleLessMore"); }
    }

    function selectionChanged(selectedList: number[]) {
        if(props.returnComplexref) {
            if(selectedList.length == 0){
                // Delete the current disaster
                switch(props.disaster) {
                    case "Meteors":
                        props.returnComplexref.current.Disasters = {
                            ...props.returnComplexref.current.Disasters,
                            Meteors: undefined
                        }
                        break;
                    case "Cold Waves":
                        props.returnComplexref.current.Disasters = {
                            ...props.returnComplexref.current.Disasters,
                            ColdWaves: undefined
                        }
                        break;
                    case "Dust Storms":
                        props.returnComplexref.current.Disasters = {
                            ...props.returnComplexref.current.Disasters,
                            DustStorms: undefined
                        }
                        break;
                    case "Dust Devils":
                        props.returnComplexref.current.Disasters = {
                            ...props.returnComplexref.current.Disasters,
                            DustDevils: undefined
                        }
                        break;
                }

                // Check if there are any other complex resources. If not, delete the Resources 'node'
                if(props.returnComplexref.current.Disasters) {
                    // using Object.keys() still returns a value above 0. Need to find a better way than this
                    if(JSON.stringify(props.returnComplexref.current.Disasters) === "{}") {
                        props.returnComplexref.current.Disasters = undefined;
                    }
                }
                props.selectionChanged();
                return;
            }
            switch(props.disaster) {
                case "Meteors":
                    props.returnComplexref.current.Disasters = {
                        ...props.returnComplexref.current.Disasters,
                        Meteors: { Number: selectedList[0], isLessThan: isLessState }
                    }
                    break;
                case "Cold Waves":
                    props.returnComplexref.current.Disasters = {
                        ...props.returnComplexref.current.Disasters,
                        ColdWaves: { Number: selectedList[0], isLessThan: isLessState }
                    }
                    break;
                case "Dust Storms":
                    props.returnComplexref.current.Disasters = {
                        ...props.returnComplexref.current.Disasters,
                        DustStorms: { Number: selectedList[0], isLessThan: isLessState }
                    }
                    break;
                case "Dust Devils":
                    props.returnComplexref.current.Disasters = {
                        ...props.returnComplexref.current.Disasters,
                        DustDevils: { Number: selectedList[0], isLessThan: isLessState }
                    }
                    break;
            }
        } else {
            refError("DropComResource");
        }
        props.selectionChanged();
    }

    return (
        <>
        <div className={props.classNames + " DropComplex"}>
            <h6>{props.disaster}</h6>
            <div id="Dropdown">
                <LessMoreButton isLess={isLessState} clickAction={toggleLessMore} />
                <Multiselect
                isObject={false}
                onKeyPressFn={function noRefCheck(){}}
                onRemove={selectionChanged}
                onSearch={function noRefCheck(){}}
                onSelect={selectionChanged}
                options={[...Array(5).keys()].slice(1,5)}
                singleSelect
                placeholder={props.disaster}
                customCloseIcon={<>❌</>}
                />
            </div>
        </div>
        </>
    );
}

/* Specific MAP DETAIL dropdowns */
export const DropComMapLanding = (props: DropdownProps) => {

    function selectionChanged(selectedList: NamedLandingSystem[]) {
        if(props.returnComplexref) {
            if(selectedList.length == 0){
                // Delete the Named Area attribute
                props.returnComplexref.current.Map = {
                    ...props.returnComplexref.current.Map,
                    NamedArea: undefined
                }
                // Check to see if the base .Map needs to be deleted
                if(JSON.stringify(props.returnComplexref.current.Map) === "{}") {
                    props.returnComplexref.current.Map = undefined;
                }
                props.selectionChanged();
                return;
            }
            // Add to complex ref
            let currList: string[] = [];
            // Create a landing area list from the selectedList
            for (let i = 0; i < selectedList.length; i++) {
                currList.push(selectedList[i].system);
            }
            // Overwrite/set the current list
            props.returnComplexref.current.Map = {
                ...props.returnComplexref.current.Map,
                NamedArea: currList};
        } else {
            refError("DropComMapLanding");
        }
        props.selectionChanged();
    }

    return (
        <>
        <div className={props.classNames + " DropComplex"}>
            <h6>Named Landing Area</h6>
            <div id="Dropdown">
                <Multiselect
                onKeyPressFn={function noRefCheck(){}}
                onRemove={selectionChanged}
                onSearch={function noRefCheck(){}}
                onSelect={selectionChanged}
                displayValue="locationName"
                options={listNamedLanding}
                placeholder="Named Landing Area"
                />
            </div>
        </div>
        </>
    );
}

export const DropComMapTopography = (props: DropdownProps) => {

    function selectionChanged(selectedList: TopographySystem[]) {
        if(props.returnComplexref) {
            if(selectedList.length == 0){
                // Delete the Named Area attribute
                props.returnComplexref.current.Map = {
                    ...props.returnComplexref.current.Map,
                    Topography: undefined
                }
                // Check to see if the base .Map needs to be deleted
                if(JSON.stringify(props.returnComplexref.current.Map) === "{}") {
                    props.returnComplexref.current.Map = undefined;
                }
                props.selectionChanged();
                return;
            }
            // Add to complex ref
            let currList: string[] = [];
            // Create a landing area list from the selectedList
            for (let i = 0; i < selectedList.length; i++) {
                currList.push(selectedList[i].system);
            }
            // Overwrite/set the current list
            props.returnComplexref.current.Map = {
                ...props.returnComplexref.current.Map,
                Topography: currList};
        } else {
            refError("DropComMapTopography");
        }
        props.selectionChanged();
    }

    return (
        <>
        <div className={props.classNames + " DropComplex"}>
            <h6>Topography</h6>
            <div id="Dropdown">
                <Multiselect
                onKeyPressFn={function noRefCheck(){}}
                onRemove={selectionChanged}
                onSearch={function noRefCheck(){}}
                onSelect={selectionChanged}
                displayValue="topographyName"
                options={listTopography}
                placeholder="Topography"
                />
            </div>
        </div>
        </>
    );
}

export const DropComMapName = (props: DropdownProps) => {

    function selectionChanged(selectedList: MapNameSystem[]) {
        if(props.returnComplexref) {
            if(selectedList.length == 0){
                // Delete the Named Area attribute
                props.returnComplexref.current.Map = {
                    ...props.returnComplexref.current.Map,
                    MapName: undefined
                }
                // Check to see if the base .Map needs to be deleted
                if(JSON.stringify(props.returnComplexref.current.Map) === "{}") {
                    props.returnComplexref.current.Map = undefined;
                }
                props.selectionChanged();
                return;
            }
            // Add to complex ref
            let currList: string[] = [];
            // Create a landing area list from the selectedList
            for (let i = 0; i < selectedList.length; i++) {
                currList.push(selectedList[i].system);
            }
            // Overwrite/set the current list
            props.returnComplexref.current.Map = {
                ...props.returnComplexref.current.Map,
                MapName: currList};
        } else {
            refError("DropComMapName");
        }
        props.selectionChanged();
    }

    return (
        <>
        <div className={props.classNames + " DropComplex"}>
            <h6>Map Name</h6>
            <div id="Dropdown">
                <Multiselect
                onKeyPressFn={function noRefCheck(){}}
                onRemove={selectionChanged}
                onSearch={function noRefCheck(){}}
                onSelect={selectionChanged}
                displayValue="mapName"
                options={listMapNames}
                placeholder="Map Name"
                />
            </div>
        </div>
        </>
    );
}

export const DropComMapChallenge = (props: DropdownProps) => {
    const [isLessState, setIsLessState] = useState(false);

    function toggleLessMore() {
        setIsLessState(!isLessState);
        // Set isLessTHan
        if(props.returnComplexref && props.returnComplexref.current.Map) {
            if(props.returnComplexref.current.Map.Challenge) {
                props.returnComplexref.current.Map.Challenge = {
                    ...props.returnComplexref.current.Map.Challenge,
                    isLessThan: !isLessState
                }
            }
        }
    }

    function blankSearch() {
        if(props.returnComplexref) {
            // Delete the Named Area attribute
            props.returnComplexref.current.Map = {
                ...props.returnComplexref.current.Map,
                Challenge: undefined
            }
            // Check to see if the base .Map needs to be deleted
            if(JSON.stringify(props.returnComplexref.current.Map) === "{}") {
                props.returnComplexref.current.Map = undefined;
            }
            props.selectionChanged();
            return;
        } else {
            refError("DropComMapName");
        }
        props.selectionChanged();
    }

    function selectionChanged(selectedList: number[]) {
        if(props.returnComplexref) {
            // Overwrite/set the current list
            props.returnComplexref.current.Map = {
                ...props.returnComplexref.current.Map,
                Challenge: { Number: selectedList[0], isLessThan: isLessState}};
        } else {
            refError("DropComMapName");
        }
        props.selectionChanged();
    }
    
    return (
        <>
        <div className={props.classNames + " DropComplex"}>
            <h6>Challenge Difficulty</h6>
            <div id="Dropdown">
            <LessMoreButton isLess={isLessState} clickAction={toggleLessMore} />
            <Multiselect
                isObject={false}
                onKeyPressFn={function noRefCheck(){}}
                onRemove={blankSearch}
                onSearch={function noRefCheck(){}}
                onSelect={selectionChanged}
                options={[100, 120, 140, 160, 180, 200, 220, 240]}
                singleSelect
                placeholder="Challenge"
                customCloseIcon={<>❌</>}
            />
            </div>
        </div>
        </>
    );
}

/* Coordinate Dropdown */
export const DropCoordinateVariant =  (props: DropdownProps) => {

    const firstRun = useRef(true);

    if(firstRun.current) {
        // Set the default variant
        if(props.returnCoordinateRef) {
            props.returnCoordinateRef.current = listVariantSYSTEM[0].system;
        } else {
            refError("DropGameVariant");
        }
        firstRun.current = false;
    }

    function selectionChanged(selectedList: VariantSystem[]) {
        if(props.returnCoordinateRef) {
            props.returnCoordinateRef.current = selectedList[0].system;
        } else {
            refError("DropGameVariant");
        }
        props.selectionChanged();
    }

    return (
        <>
        <div className={props.classNames}>
            <div id="WholeWidth">
            <Multiselect
                onKeyPressFn={function noRefCheck(){}}
                onRemove={function noRefCheck(){}}
                onSearch={function noRefCheck(){}}
                onSelect={selectionChanged}
                displayValue="variant"
                options={listVariantSYSTEM}
                singleSelect
                placeholder="Variant"
                selectedValues={[listVariantSYSTEM[0]]}
                customCloseIcon={<></>}
            />
            </div>
        </div>
        </>
    );
}