import { useState, useRef, MutableRefObject } from 'react'

import {SearchRow} from './searchRow'
import { Breakthrough, Variant } from './SM-Constants'

  // AppBody:{
  // Search Parameters
  // item(s) View
  // Result view
  // }
export default function appMain() {

    type SelectedSearch = {
        simpleString: string;
        complexString: string;
    }

    const simpleStringRef: MutableRefObject<string> = useRef("");
    const complexStringRef: MutableRefObject<string> = useRef("");

    const searchState : SelectedSearch = { simpleString: "", complexString: ""};

    return (
        <>
        <div id="AppMain" className="container col-8">
            <div className="search" id="Simple">
                <SearchRow collapsed={false} collapses={false} searchType={"Simple"} returnStringRef={simpleStringRef}/>
            </div>
            <hr/>
            <div className="search" id="Complex">
                <SearchRow collapsed={true} collapses={true} searchType={"Complex"} returnStringRef={complexStringRef}/>
            </div>
            <hr/>
            <div className="row d-grid mx-auto justify-content-md-center" id="Search">
                <button type="button" className="btn btn-lg btn-success">Search</button>
            </div>
            <hr/>
            <div className="row view" id="Items-View">Item(s)</div>
            <hr/>
            <div className="row" id="Sorting">Sorting</div>
            <hr/>
            <div className="row results" id="Results">Results</div>
        </div>
        </>
    )
}