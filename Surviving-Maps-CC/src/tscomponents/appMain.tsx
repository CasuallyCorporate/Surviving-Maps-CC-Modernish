import { useState } from 'react'

import { Breakthrough, Variant } from './SM-Consts'

  // AppBody:{
  // Search Parameters
  // item(s) View
  // Result view
  // }
export default function appMain() {

    type SelectedSearch = {
        searchstring: string;
        variant: Variant;
        breakthroughs: Breakthrough[];
    }


    return (
        <>
        <div id="AppMain" className="container col-8">
            <div className="row" id="simple">Simple</div>
            <div className="row" id="complex">Complex</div>

            <div className="row view" id="items-view">Item(s)</div>

            <div className="row" id="sorting">Sorting</div>

            <div className="row results" id="results">Results</div>
        </div>
        </>
    )
}