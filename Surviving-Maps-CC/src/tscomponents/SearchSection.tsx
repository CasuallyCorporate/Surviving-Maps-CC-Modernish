import { useState, useRef, MutableRefObject } from "react";
import { SearchRow } from "./searchRow";
import { DropBreakthrough, DropDisasters, DropGameVariant, DropResources } from "./Dropdowns";
import { ComplexRequest, SimpleRequest } from "./SM-Constants";

interface SearchSectionProps {
    collapsed: Boolean;
    collapses: Boolean;
    rowName: String;
    changeContents: Boolean;
    returnSimpleRef?: MutableRefObject<SimpleRequest>;
    returnComplexRef?: MutableRefObject<ComplexRequest>;
    toggled(name: String, changedTo: Boolean): any;
    changedSelection(): any;
}
export const SearchSection = (props: SearchSectionProps) => {

    const canCollapse = useRef(props.collapses);
    const [isCollapsed, setCollapsed] = useState(props.collapsed);

    function toggleCollapsed() {
        if(canCollapse.current){
            const newCollapse: Boolean = !isCollapsed;
            // set the current collapse opposite to
            setCollapsed(newCollapse);
            // inform the main of the new value
            props.toggled(props.rowName, newCollapse);
        };
    }

    if(isCollapsed){
        // Collapsed: All searchComponents inside will not be shown
        return (
            <>
            <a className="user-select-none" id="RowTitle" onClick={() => toggleCollapsed()}>
                <div id="collapseIcon" className={ 
                    canCollapse.current ? (isCollapsed? "triangle" : "rectangle") : "circle"
                    }></div>
                <h3>{props.rowName}</h3>
            </a>
            </>
        );
    }
    
    // Change depending on input for each search row type
    switch(props.rowName) {
        case "Simple":
            if(props.returnSimpleRef){
                if (props.changeContents) {
                    return (
                        <>
                        <a className="user-select-none" id="RowTitle" onClick={() => toggleCollapsed()} >
                            <div id="collapseIcon" className={ 
                                canCollapse.current ? (isCollapsed ? "triangle" : "rectangle") : "circle"
                                }></div>
                            <h3>{props.rowName}</h3>
                        </a>
                        <div id="SearchComponents">
                            <DropGameVariant classNames="col-md-3" returnSimpleRef={props.returnSimpleRef} selectionChanged={props.changedSelection} />
                        </div>
                        <div id="SearchComponents">
                            <DropBreakthrough classNames="col-md-12" returnSimpleRef={props.returnSimpleRef} selectionChanged={props.changedSelection} />
                        </div>
                        </>
                    );
                } else {
                    return (
                        <>
                        <a className="user-select-none" id="RowTitle" onClick={() => toggleCollapsed()} >
                            <div id="collapseIcon" className={ 
                                canCollapse.current ? (isCollapsed ? "triangle" : "rectangle") : "circle"
                                }></div>
                            <h3>{props.rowName}</h3>
                        </a>
                        <div id="SearchComponents">
                            <DropGameVariant classNames="col-md-3" returnSimpleRef={props.returnSimpleRef} selectionChanged={props.changedSelection} />
                            <DropResources classNames="col-md-4 slSearch" returnSimpleRef={props.returnSimpleRef} selectionChanged={props.changedSelection} />
                            <DropDisasters classNames="col-md-4 slSearch" returnSimpleRef={props.returnSimpleRef} selectionChanged={props.changedSelection} />
                        </div>
                        <div id="SearchComponents">
                            <DropBreakthrough classNames="col-md-12" returnSimpleRef={props.returnSimpleRef} selectionChanged={props.changedSelection} />
                        </div>
                        </>
                    );
                }
            }
            console.error("searchRow of name 'Simple' did not receive a returnSimpleRef");
            break;
        case "Complex":
            if(props.returnComplexRef) {
                if(isCollapsed){
                    return (
                        <>
                        <a className="user-select-none" id="RowTitle" onClick={() => toggleCollapsed()} >
                            <div id="collapseIcon" className={ 
                                canCollapse ? (isCollapsed ? "triangle" : "rectangle") : "circle"
                                }></div>
                            <h3>{props.rowName}</h3>
                        </a>
                        </>
                    );
                } else {
                    return (
                        <>
                        <a className="user-select-none" id="RowTitle" onClick={() => toggleCollapsed()} >
                            <div id="collapseIcon" className={ 
                                canCollapse ? (isCollapsed ? "triangle" : "rectangle") : "circle"
                                }></div>
                            <h3>{props.rowName}</h3>
                        </a>
                        <SearchRow name="Resources" returnComplexref={props.returnComplexRef} searchValueChanged={props.changedSelection} />
                        <SearchRow name="Disasters" returnComplexref={props.returnComplexRef} searchValueChanged={props.changedSelection} />
                        <SearchRow name="Map Details" returnComplexref={props.returnComplexRef} searchValueChanged={props.changedSelection} />
                        </>
                    );
                }
            }
            console.error("searchRow of name 'Complex' did not receive a returnComplexRef");
            break;
        default:
            return (
                <>
                <a className="user-select-none" onClick={() => toggleCollapsed()} >
                    <div id="collapseIcon" className={ 
                        canCollapse ? (isCollapsed ? "triangle" : "rectangle") : "circle"
                        }></div>
                    <h3>{props.rowName}</h3>
                </a>
                <div id="SearchComponents">
                    <p>You shouldn't be able to read this...</p>
                    <p>Either something has gone wrong, or you are up to something... 🤔</p>
                </div>
                </>
            );
    }
};