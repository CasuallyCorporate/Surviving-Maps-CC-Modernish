import { MutableRefObject } from "react";
import { DropComDisaster, DropComMapChallenge, DropComMapLanding, DropComMapName, DropComMapTopography, DropComResource } from "./Dropdowns";
import { ComplexRequest } from "./SM-Constants";

interface SearchRowProps {
    name: string;
    returnComplexref: MutableRefObject<ComplexRequest>;
    searchValueChanged(): any;
}

export const SearchRow = (props: SearchRowProps) => {
    switch(props.name) {
        case "Resources":
            return (
                <>
                <div className="SearchRow">
                    <h5>{props.name}</h5>
                    <hr></hr>
                    <div id="SearchComponents">
                    <DropComResource classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged} resource="Water"/>
                    <DropComResource classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged} resource="Concrete"/>
                    <DropComResource classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged} resource="Metal"/>
                    <DropComResource classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged} resource="Rare Metal"/>
                    </div>
                </div>
                </>
            );
            break;
        case "Disasters":
            return (
                <>
                <div className="SearchRow">
                    <h5>{props.name}</h5>
                    <hr></hr>
                    <div id="SearchComponents">
                    <DropComDisaster classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged} disaster="Meteors"/>
                    <DropComDisaster classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged} disaster="Cold Waves"/>
                    <DropComDisaster classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged} disaster="Dust Storms"/>
                    <DropComDisaster classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged} disaster="Dust Devils"/>
                    </div>
                </div>
                </>
            );
            break;
        case "Map Details":
            return (
                <>
                <div className="SearchRow">
                    <h5>{props.name}</h5>
                    <hr></hr>
                    <div id="SearchComponents">
                    <DropComMapLanding classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged} />
                    <DropComMapTopography classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged}  />
                    <DropComMapName classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged} />
                    <DropComMapChallenge classNames="col-md-3" returnComplexref={props.returnComplexref} selectionChanged={props.searchValueChanged} />
                    </div>
                </div>
                </>
            );
            break;
        default:
            return (
                <>
                <div className="SearchRow">
                    <h5>Error</h5>
                    <hr></hr>
                    <div>
                    Nothing
                    </div>
                </div>
                </>
            );
    }
}