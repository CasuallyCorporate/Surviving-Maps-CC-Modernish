import { MutableRefObject, useRef, useState } from 'react';
import { PageResponse, PageSite } from './SM-Constants';

export interface sendResultsObj {
    resultSend: boolean;
}

interface ResultItemProps {
    listNum: number;
    siteDetails: PageSite;
    notifyItemClick(siteID: number): any;
    sendItemRef: MutableRefObject<sendResultsObj>;
}

export const ResultItem = (props: ResultItemProps) => {
    const itemSiteID: MutableRefObject<number> = useRef(props.siteDetails.ID);
    const [isSelected, setSelected] = useState<boolean>(false);

    function clicked() {
       if(isSelected) {
        // The item is selected. Unselect
        setSelected(!isSelected);
       } else {
        // The item is not selected
        if(props.sendItemRef.current.resultSend) {
            // If we can send. Do so and set selected
            props.notifyItemClick(itemSiteID.current);
            // Toggle selected
            setSelected(!isSelected);
            props.sendItemRef.current.resultSend = false;
        }
        // Otherwise do nothing
       }
    }

    if(isSelected) {
        return(
            <>
            <a className="SiteItem siteSelected" onClick={clicked}>
                <div className="col-md-1">{props.siteDetails.Coordinates}</div>
                <div className="col-md-1">{props.siteDetails.DisastersTot}</div>
                <div className="col-md-1">{props.siteDetails.ResourcesTot}</div>
    
                <div className="col-md-1">{props.siteDetails.DustDevils}</div>
                <div className="col-md-1">{props.siteDetails.DustStorms}</div>
                <div className="col-md-1">{props.siteDetails.Meteors}</div>
                <div className="col-md-1">{props.siteDetails.ColdWaves}</div>
    
                <div className="col-md-1">{props.siteDetails.Concrete}</div>
                <div className="col-md-1">{props.siteDetails.Water}</div>
                <div className="col-md-1">{props.siteDetails.Metal}</div>
                <div className="col-md-1">{props.siteDetails.RareMetal}</div>
            </a>
            </>
        );
    } else {
        return(
            <>
            <a className="SiteItem" onClick={clicked}>
                <div className="col-md-1">{props.siteDetails.Coordinates}</div>
                <div className="col-md-1">{props.siteDetails.DisastersTot}</div>
                <div className="col-md-1">{props.siteDetails.ResourcesTot}</div>
    
                <div className="col-md-1">{props.siteDetails.DustDevils}</div>
                <div className="col-md-1">{props.siteDetails.DustStorms}</div>
                <div className="col-md-1">{props.siteDetails.Meteors}</div>
                <div className="col-md-1">{props.siteDetails.ColdWaves}</div>
    
                <div className="col-md-1">{props.siteDetails.Concrete}</div>
                <div className="col-md-1">{props.siteDetails.Water}</div>
                <div className="col-md-1">{props.siteDetails.Metal}</div>
                <div className="col-md-1">{props.siteDetails.RareMetal}</div>
            </a>
            </>
        );
    }
}

interface ResultsViewProps {
    loading: boolean;
    searchedBefore: boolean;
    parentNotifyItemClick(num: number): any;
    pageResponse?: MutableRefObject<PageResponse>;
    promptGoToPage(pageNum: number): any;
    sendItemRef: MutableRefObject<sendResultsObj>;
}

export const ResultsView = (props: ResultsViewProps) => {

    const allResultItems: MutableRefObject<JSX.Element[]> = useRef([]);

    function returnElementsFromItems( Sites: PageSite[] ): JSX.Element[] {
        let elements: JSX.Element[] = [];
        let sitesLen = Sites.length;
        let listKeyNum = 0;

        for(let index = 0; index < sitesLen; index++) {
            elements.push(<ResultItem key={listKeyNum} listNum={listKeyNum} siteDetails={Sites[index]}
                notifyItemClick={props.parentNotifyItemClick} sendItemRef={props.sendItemRef} />);
            listKeyNum++;
        }

        return elements;
    }

    function goToFirstPage(){
        if(props.pageResponse){
            props.promptGoToPage(1);
        }
    }
    function goToPrevPage(){
        if(props.pageResponse){
            if(0 < (props.pageResponse.current.PageNum -1)) {
                props.promptGoToPage(props.pageResponse.current.PageNum - 1);
            }
        }
    }
    function goToNextPage(){
        if(props.pageResponse){
            if((props.pageResponse.current.PageNum + 1) < (props.pageResponse.current.PageTot + 1)) {
                props.promptGoToPage(props.pageResponse.current.PageNum + 1);
            }
        }
    }
    function goToLastPage(){
        if(props.pageResponse){
            props.promptGoToPage(props.pageResponse.current.PageTot);
        }
    }

    if(props.loading) {
        // Display the loading icon thingy
        return (
            <>
            <div className="spinner-grow loading-icon" role="status"></div>
            </>
        );
    } else {
        if(props.pageResponse) {
            if(props.pageResponse.current.PageSites.length > 0) {
                // Load the entries in
                allResultItems.current = returnElementsFromItems(props.pageResponse.current.PageSites);
                return (
                    <>
                    <div id="PageNav">
                        <div id="Page">Page: {props.pageResponse.current.PageNum} of {props.pageResponse.current.PageTot}</div>
                        <div id="PageNavButtons">
                            <a id="SkipFront" onClick={goToFirstPage}>{'<<'}</a>
                            <a id="NavFront" onClick={goToPrevPage}>{'<'}</a>
                            <a id="NavBack" onClick={goToNextPage}>{'>'}</a>
                            <a id="SkipBack" onClick={goToLastPage}>{'>>'}</a>
                        </div>
                    </div>
                    {allResultItems.current}
                    </>
                );
            } else {
                // Handle no results
                if(props.searchedBefore) {
                    return (
                        <>
                        <p>No entries in search</p>
                        </>
                    );
                } else {
                    return (
                        <>
                        <p>Search to populate this area with results</p>
                        </>
                    );
                }
            }
        } else {
            console.error("ResultsView: pageResponse Ref does not exist...");
        }
    }
}