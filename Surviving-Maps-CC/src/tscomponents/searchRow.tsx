import React, { FC ,useState, MutableRefObject } from "react";

interface SearchRowProps {
    collapsed: Boolean;
    collapses: Boolean;
    searchType: string;
    returnStringRef: MutableRefObject<string>;
    }

export const SearchRow = (props: SearchRowProps) => {

    const [canCollapse, setCollapses] = useState(props.collapses);
    const [isCollapsed, setCollapsed] = useState(props.collapsed);

    function toggleCollapsed() {       
        if(!canCollapse){return;};
        const currentCollapse = isCollapsed;
        setCollapsed(!currentCollapse);
        // if not currently collapsed, reset all search items
    }

    if(isCollapsed){
        // Collapsed: All items inside will not be shown
        return (
            <>
            <a className="user-select-none" onClick={() => toggleCollapsed()}>
                <div id="collapseIcon" className={ 
                    canCollapse ? (isCollapsed? "triangle" : "square") : "circle"
                    }></div>
                <h3>{props.searchType}</h3>
            </a>
            </>
        );
    }
    
    // Change depoending on input for each search row type
    switch(props.searchType) {
        case "Simple":
            return (
                <>
                <a className="user-select-none" onClick={() => toggleCollapsed()} >
                    <div id="collapseIcon" className={ 
                        canCollapse ? (isCollapsed ? "triangle" : "square") : "circle"
                        }></div>
                    <h3>{props.searchType}</h3>
                </a>
                <div id="SearchComponents">

                </div>
                </>
            );
            break;
        case "Complex":
            return (
                <>
                <a className="user-select-none" onClick={() => toggleCollapsed()} >
                    <div id="collapseIcon" className={ 
                        canCollapse ? (isCollapsed ? "triangle" : "square") : "circle"
                        }></div>
                    <h3>{props.searchType}</h3>
                </a>
                <div id="SearchComponents">

                </div>
                </>
            );
            break;
        default:
            return (
                <>
                <a className="user-select-none" onClick={() => toggleCollapsed()} >
                    <div id="collapseIcon" className={ 
                        canCollapse ? (isCollapsed ? "triangle" : "square") : "circle"
                        }></div>
                    <h3>{props.searchType}</h3>
                </a>
                <div id="SearchComponents">
                    <p>You shouldn't be able to read this...</p>
                    <p>Either something has gone wrong, or you are up to wrong...</p>
                </div>
                </>
            );
    }
};