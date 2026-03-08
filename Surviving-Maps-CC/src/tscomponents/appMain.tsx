import { useState, useRef, MutableRefObject } from 'react'

import {SearchSection} from './SearchSection'
import {SortingRow} from './SortingRow'

import { SimpleRequest, ComplexRequest, SortingRequest, PageRequest, PageResponse } from './SM-Constants'
import { changesStateObj, SearchButtonRow } from './SearchButtonRow';
import { ResultsView, sendResultsObj } from './ResultsView';
import { ItemToView, ItemViewRow } from './ItemViewRow';


export interface SortingEnabled {
    isEnabled: boolean;
    sortingReq?: SortingRequest;
}

  // AppBody:{
  // Search Parameters
  // item(s) View
  // Search Button
  // The sort
  // Result view
  // }
export default function appMain() {

    const simpleRef: MutableRefObject<SimpleRequest> = useRef({});
    const complexRef: MutableRefObject<ComplexRequest> = useRef({});
    const sortRef: MutableRefObject<SortingEnabled> = useRef({isEnabled: false});
    const lastFullRequest: MutableRefObject<PageRequest> = useRef({Simple:{}});

    const [isSimpleChanged, setSimpleChanged] = useState(false);

    function returnFullRequest(noSet?: string): PageRequest {
        let tmpFullRequest: PageRequest = {Simple: simpleRef.current};
        if(!(JSON.stringify(complexRef.current) === "{}")) {
            tmpFullRequest.Complex = complexRef.current;
        }
        if(sortRef.current.isEnabled) {
            // Check to see if the object is actually there
            if(sortRef.current.sortingReq) {
                tmpFullRequest.Sorting = sortRef.current.sortingReq;
            } else {
                console.error("sortRef had isEnabled, but sortRef..sortingReq was disabled/not there....");
            }
        }

        if(noSet && noSet == "noSet") {
            // noSet only return
            return tmpFullRequest;
        }

        // Set the last search parameters as the currently retrieved parameters
        lastSearchParameters.current = JSON.stringify(tmpFullRequest);
        // Also save the item for use
        lastFullRequest.current = tmpFullRequest;

        return tmpFullRequest;
    }

    function rowToggled(rowName: string, changedTo: boolean) {
        switch(rowName) {
            case "Complex":
                // Changed
                if (changedTo) {
                    // Changed to Closed
                    // Reset thhe complex resources and disasters
                    complexRef.current.Resources = undefined;
                    complexRef.current.Disasters = undefined;
                    complexRef.current.Map = undefined;
                    setSimpleChanged(false);
                }else {
                    // changed to open
                    // Reset the simple resources and disasters
                    simpleRef.current.Resources = undefined;
                    simpleRef.current.Disasters = undefined;
                    setSimpleChanged(true);
                }
            break;
            default:
                console.error("Error: Given rowName that is not in list");
            break;
        }
    }

    // Consts for handling search results
    const [isLoadingResults, setIsLoadingResults] = useState(false);
    const pageResponseRef: MutableRefObject<PageResponse> = useRef({ VariantSystem: "", PageNum: -1, PageTot: -1, PageSites: [] });
    const defPage: number = -1;
    const [getSpecificPage, setSpecificPage] = useState(defPage);
    const haveSearchedBefore: MutableRefObject<boolean> = useRef(false);

    const lastSearchParameters = useRef<String>( "{\"Simple\":{\"Variant\":\"STANDARD\"}}");
    const [searchValuesChanged, setSearchValuesChanged] = useState<changesStateObj>({toggle: false, changed: false});

    function handleSearchValueChanged(){

        if(lastSearchParameters.current === JSON.stringify(returnFullRequest("noSet"))) {
            //Last search params, and current params are the same
            //setSearchChanged to false
            console.debug("Last and new search params are the same");
            setSearchValuesChanged({toggle: !searchValuesChanged.toggle, changed: false});
        } else {
            //Last search params, and current params are different
            //setSearchChanged to true
            console.debug("Last and new search params are different");
            setSearchValuesChanged({toggle: !searchValuesChanged.toggle, changed: true});
        }
    }

    function setLoading(setLoading: boolean){
        setIsLoadingResults(setLoading);
        // Also presume search has been pressed
        haveSearchedBefore.current = true;
    }

    function handleSearchResults(response: PageResponse) {
        pageResponseRef.current = response;
    }

    const sendItemRef: MutableRefObject<sendResultsObj> = useRef({ resultSend: true });
    const [newItemToView, setNewItemToView] = useState<ItemToView>({toggled: false, VariantSystem:"", siteID: -2});
    function handleItemClicked(IDNum: number) {
        // Get variant and ID together
        // trigger ItemView with new [Variant,SiteID]
        if(lastFullRequest.current.Simple.Variant) {
            setNewItemToView({toggled: newItemToView.toggled, VariantSystem: lastFullRequest.current.Simple.Variant, siteID: IDNum});
        } else {
            console.error("MainApp: handleItemClicked: lastFullRequest.Simple.Variant did not exist...");
        }
    }

    function handleGoToPage(pagenum: number) {
        //goto Page
        setSpecificPage(pagenum);
    }

    return (
        <>
        <div id="AppMain" className="container col-md-9">
            <div className="search" id="Simple">
                <SearchSection collapsed={false} collapses={false} rowName={"Simple"} changeContents={isSimpleChanged} returnSimpleRef={simpleRef}
                toggled={rowToggled} changedSelection={handleSearchValueChanged} />
            </div>
            <hr/>
            <div className="search" id="Complex">
                <SearchSection collapsed={true} collapses={true} rowName={"Complex"} changeContents={false} returnComplexRef={complexRef}
                toggled={rowToggled} changedSelection={handleSearchValueChanged} />
            </div>
            <hr/>
            <div id="Search">
                <SearchButtonRow getFullRequest={returnFullRequest} setLoadingAnimation={setLoading} returnsResults={handleSearchResults}
                specificPage={getSpecificPage} changedSearchValues={searchValuesChanged} />
            </div>
            <hr/>
            <div className="col" id="Items-View">
                <ItemViewRow newitem={newItemToView} sendItemRef={sendItemRef} />
            </div>
            <hr/>
            <div className="row" id="Sorting">
                <SortingRow returnSortingEnabled={sortRef} changedSelection={handleSearchValueChanged} />
            </div>
            <hr/>
            <div className="row" id="Results">
                <ResultsView loading={isLoadingResults} searchedBefore={haveSearchedBefore.current} parentNotifyItemClick={handleItemClicked} pageResponse={pageResponseRef}
                promptGoToPage={handleGoToPage} sendItemRef={sendItemRef} />
            </div>
        </div>
        </>
    );
}