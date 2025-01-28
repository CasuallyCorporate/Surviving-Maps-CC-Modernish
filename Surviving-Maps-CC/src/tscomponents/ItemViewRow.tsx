import { MutableRefObject, useEffect, useRef, useState } from "react";
import { Breakthrough, BreakthroughList, CoordinateObj, listBreakthroughSYSTEM, listNamedLanding, listTopography, listVariantSYSTEM, SiteRequest, SiteResponse } from "./SM-Constants";
import { ResultsView, sendResultsObj } from "./ResultsView";
import { ItemViewUtiltyCoordinates, ItemViewUtiltyFromBrowser, listBrowserPassthrough, UtilityOpenClose } from "./ItemViewUtilities";

interface siteThumbProps {
    site: SiteResponse;
    parentDeeleteMe(site: SiteResponse): any;
    parentShowMe(site: SiteResponse): any;
}
const SiteThumbnail = (props: siteThumbProps) => {

    function findTopographyNice(sysTopography: string): string {
        let listLen: number = listTopography.length;
        for(let i = 0; i < listLen; i++) {
            if(listTopography[i].system == sysTopography) {
                return listTopography[i].topographyName;
                break;
            }
        }
        console.log("DisplaySite: findVariantNice: Did not find variant");
        return "Error";
    }
    function findVariantNice(sysVariant: string): String {
        let listLen: number = listVariantSYSTEM.length;
        for(let i = 0; i < listLen; i++) {
            if(listVariantSYSTEM[i].system == sysVariant) {
                return listVariantSYSTEM[i].variant.toString();
                break;
            }
        }
        console.log("DisplaySite: findVariantNice: Did not find variant");
        return "Error";
    }
    function translateCoordinate(coord: CoordinateObj): string {
        let strReturn = "";
        strReturn += coord.NSNum;
        strReturn += coord.NSChar;
        strReturn += ":";
        strReturn += coord.EWNum;
        strReturn += coord.EWChar;
        return strReturn;
    }

    function localDeleteMe(){
        props.parentDeeleteMe(props.site);
    }
    function localShowMe() {
        props.parentShowMe(props.site);
    }

    return (
        <>
        <div className="siteThumb col-md-3">
            <div className="Headings">
                <h6>{findVariantNice(props.site.VariantSystem)} | {translateCoordinate(props.site.Coordinate)} {'-'}{findTopographyNice(props.site.TopographySYS)}</h6>
                <a onClick={localDeleteMe}>❌</a>
            </div>
            <a className="body" onClick={localShowMe}>
                <div className="MapThumb">
                    <img alt="map" src={"/maps/" + props.site.MapName + ".png"}/>
                </div>
                <div className="DisastersResources">
                    <div className="Disasters">
                        <div className="icon-resource icon-devils">{props.site.DustDevils}</div>
                        <div className="icon-resource icon-storms">{props.site.DustStorms}</div>
                        <div className="icon-resource icon-meteors">{props.site.Meteors}</div>
                        <div className="icon-resource icon-cold">{props.site.ColdWaves}</div>
                    </div>
                    <div className="Resources">
                        <div className="icon-resource icon-concrete">{props.site.Concrete}</div>
                        <div className="icon-resource icon-water">{props.site.Water}</div>
                        <div className="icon-resource icon-metal">{props.site.Metal}</div>
                        <div className="icon-resource icon-raremetal">{props.site.RareMetal}</div>
                    </div>
                </div>
            </a>
        </div>
        </>
    );
}

interface addSiteObj {
    toggle: boolean;
    toAdd?: SiteResponse;
}
interface siteListProps {
    addSite: addSiteObj;
    informParentItemSelected(site: SiteResponse): any;
    siteList: MutableRefObject<Array<SiteResponse>>;
}

const SiteList = (props: siteListProps) => {

    function translateCoordinate(coord: CoordinateObj): string {
        let strReturn = "";
        strReturn += coord.NSNum;
        strReturn += coord.NSChar;
        strReturn += ":";
        strReturn += coord.EWNum;
        strReturn += coord.EWChar;
        return strReturn;
    }

    const [siteListElements, setSiteListElements] = useState<Array<JSX.Element>>([]);

    function listRedo() {
        // Redo the entire list
        let tmpSiteElems: Array<JSX.Element> = [];
        let tmpItemLen = props.siteList.current.length;
        for(let i = 0; i < tmpItemLen; i++) {
            tmpSiteElems.push(<SiteThumbnail parentShowMe={showSiteInDisplay} site={props.siteList.current[i]} parentDeeleteMe={deleteSite}
                key={props.siteList.current[i].VariantSystem + translateCoordinate(props.siteList.current[i].Coordinate)} />);
        }
        setSiteListElements(tmpSiteElems);
        
        // Display list
        // console.log("AfterAdd: tmp:");
        /*
        for(let i = 0; i < tmpItemLen; i++) {
            console.log(props.siteList.current[i].VariantSystem + "|" + translateCoordinate(props.siteList.current[i].Coordinate));
        }
        */
    }

    function addSite(addSite: SiteResponse) {
        // Check if it already exists first via variant/coords
        let tmpItemLen: number = props.siteList.current.length;
        for(let i = 0; i < tmpItemLen; i++) {
            if(props.siteList.current[i].VariantSystem == addSite.VariantSystem){
                if(props.siteList.current[i].Coordinate == addSite.Coordinate){
                    // Exists return. Do not add
                    return;
                }
            }
        }

        // Add to the array
        props.siteList.current.splice(0,0,addSite);
    }

    function showSiteInDisplay(site: SiteResponse) {
        props.informParentItemSelected(site);
    }

    function deleteSite(delSite: SiteResponse) {
        // Check for the site via variant/coords
        let tmpItemLen: number = props.siteList.current.length;
        let foundNum: number = -1;
        for(let i = 0; i < tmpItemLen; i++) {
            if(props.siteList.current[i].VariantSystem == delSite.VariantSystem){
                if(props.siteList.current[i].Coordinate == delSite.Coordinate){
                    // Exists
                    foundNum = i;
                    break;
                }
            }
        }
        
        if(foundNum < 0) { return; }

        if(foundNum == 0) { props.siteList.current.shift(); }
        else if(foundNum == tmpItemLen) { props.siteList.current.pop(); }
        else { props.siteList.current.splice(foundNum,1); }

        // Redo the entire list
        listRedo();
    }

    // Site added from parent
    useEffect(() => {
        if(props.addSite.toAdd) {
            addSite(props.addSite.toAdd);
        }
        listRedo();
    },[props.addSite]);

    return (
        <>
        <div className="viewItemList">
            <div className="Thumbnails">
                {siteListElements}
            </div>
        </div>
        </>
    );
}

interface btrDisplayProps {
    missing: number;
    breakthroughs: string[];
}
const BreakthroughDisplay = (props: btrDisplayProps) => {

    let btrElems: JSX.Element[] = [];

    // translate all breakthroughs to their "nice" versions before display
    let btrLen = props.breakthroughs.length;
    let constLen = listBreakthroughSYSTEM.length;
    for(let btrInd = 0; btrInd < btrLen; btrInd++) {
        // For every incoming btr, match to the 'Nice' version
        let found: boolean = false;
        for(let i = 0; i < constLen; i++) {
            if(listBreakthroughSYSTEM[i] == props.breakthroughs[btrInd]) {
                btrElems.push(<p key={i}>{BreakthroughList[i]}</p>);
                found = true;
                break;
            }
        }
        if(!found) { console.error("ItemViewRow: BreakthroughDisplay: Did not find 'Nice' name for: " + props.breakthroughs[btrInd]); }
    }

    if(props.missing > 0) {
        btrElems.splice(0,0,
            <div key={"NOTE"} className="tool-tipped">
                <p className="Note">{'<---NOTE--->'}</p>
                <p className="hover-tooltip">Due to the game variant, the first {props.missing} Breakthroughs are not searchable</p>
            </div>
            );
        return (
            <>
            <div className="BreakthroughData col-md-3">
                {btrElems}
            </div>
            </>
        );
    } else {
        if(btrElems.length > 4){
            btrElems.splice(4,0,
            <div key={"NOTE"} className="tool-tipped">
                <p className="Note">{'<---NOTE--->'}</p>
                <p className="hover-tooltip">Breakthroughs 1-4 are Planetary anomalies</p>
            </div>
            );
        }
        return (
            <>
            <div className="BreakthroughData col-md-3">
                {btrElems}
            </div>
            </>
        );
    }
}

interface DisplaySiteProps {
    ingestResponse: SiteResponse;
    sendSiteToList(toSend: SiteResponse): any;
}
const DisplaySite = (props: DisplaySiteProps) => {

    function findVariantNice(sysVariant: string): String {
        let listLen: number = listVariantSYSTEM.length;
        for(let i = 0; i < listLen; i++) {
            if(listVariantSYSTEM[i].system == sysVariant) {
                return listVariantSYSTEM[i].variant.toString();
                break;
            }
        }
        console.log("DisplaySite: findVariantNice: Did not find variant");
        return "Error";
    }
    function translateCoordinate(coord: CoordinateObj): string {
        let strReturn = "";
        strReturn += coord.NSNum;
        strReturn += coord.NSChar;
        strReturn += ":";
        strReturn += coord.EWNum;
        strReturn += coord.EWChar;
        return strReturn;
    }
    function findTopographyNice(sysTopography: string): string {
        let listLen: number = listTopography.length;
        for(let i = 0; i < listLen; i++) {
            if(listTopography[i].system == sysTopography) {
                return listTopography[i].topographyName;
                break;
            }
        }
        console.log("DisplaySite: findVariantNice: Did not find variant");
        return "Error";
    }
    function findNamedLandingNice(sysLanding: string): string {
        let listLen: number = listNamedLanding.length;
        for(let i = 0; i < listLen; i++) {
            if(listNamedLanding[i].system == sysLanding) {
                return listNamedLanding[i].locationName;
                break;
            }
        }
        console.log("DisplaySite: findNamedLandingNice: Did not find variant");
        return "Error";
    }

    function localSendToList() {
        props.sendSiteToList(props.ingestResponse);
    }

    return (
        <>
        <div className="DisplayItem">
            <div className="Display">
                <div className="Headings col-md-12">
                    <h4>{findVariantNice(props.ingestResponse.VariantSystem)} | {translateCoordinate(props.ingestResponse.Coordinate)}</h4>
                    <a className="btn btn-light" onClick={localSendToList}><p>Add to list</p></a>
                </div>
                <div className="Map col-md-6">
                    <img alt="map preview" src={"/maps/" + props.ingestResponse.MapName + ".png"}/>
                </div>
                <div className="MapData col-md-3">
                    <div className="strings">
                        <div className="d-flex justify-content-between">
                        <h6>Topography:</h6><div>{findTopographyNice(props.ingestResponse.TopographySYS)}</div>
                        </div>
                        <div className="d-flex justify-content-between">
                        <h6>Altitude:</h6><div>{props.ingestResponse.Altitude + " m"}</div>
                        </div>
                        <div className="d-flex justify-content-between">
                        <h6>Temperature:</h6><div>{props.ingestResponse.Temperature + " °C"}</div>
                        </div>
                        <div className="d-flex justify-content-between">
                        <h6>Difficulty:</h6><div>{props.ingestResponse.Challenge}</div>
                        </div>
                        <div className="d-flex justify-content-between">
                        <h6>Named Location:</h6><div>{findNamedLandingNice(props.ingestResponse.NamedLandingSYS)}</div>
                        </div>
                    </div>
                    <div className="icons d-flex justify-content-between">
                        <div className="disasters row">
                            <h6>Disasters:</h6>
                            <div className="icon-resource icon-devils">{props.ingestResponse.DustDevils}</div>
                            <div className="icon-resource icon-storms">{props.ingestResponse.DustStorms}</div>
                            <div className="icon-resource icon-meteors">{props.ingestResponse.Meteors}</div>
                            <div className="icon-resource icon-cold">{props.ingestResponse.ColdWaves}</div>
                        </div>
                        <div className="resources row">
                            <h6>Resources:</h6>
                            <div className="icon-resource icon-concrete">{props.ingestResponse.Concrete}</div>
                            <div className="icon-resource icon-water">{props.ingestResponse.Water}</div>
                            <div className="icon-resource icon-metal">{props.ingestResponse.Metal}</div>
                            <div className="icon-resource icon-raremetal">{props.ingestResponse.RareMetal}</div>
                        </div>
                    </div>
                </div>
                <BreakthroughDisplay missing={props.ingestResponse.MissingBreakthroughs} breakthroughs={props.ingestResponse.Breakthroughs} />
            </div>
        </div>
        </>
    );
}

export interface ItemToView {
    toggled: boolean;
    VariantSystem: string;
    siteID: number;
}

export interface ItemViewProps {
    newitem: ItemToView;
    sendItemRef: MutableRefObject<sendResultsObj>;
}

export const ItemViewRow = (props: ItemViewProps) => {
    const APIURL: string = "/site";

    const listSites: MutableRefObject<Array<SiteResponse>> = useRef([]);
    const [siteListPassthrough, setSiteListPassthrough] = useState<addSiteObj>({toggle: false});

    const [displayElement, setDisplayElement] = useState<JSX.Element>(
    <div className="emptyView">
        <p>Select a landing site from a search to display more information</p>
    </div>
    );

    function acceptNewDisplayItem(item: SiteResponse) {
        setDisplayElement(<DisplaySite ingestResponse={item} sendSiteToList={sendNewSiteToList} />);
    }

    function sendNewSiteToList(site: SiteResponse) {
        setSiteListPassthrough({toggle: !siteListPassthrough.toggle, toAdd: site});
    }
    
    /* Fetch and timeout stuff */
    async function fetchSite(apiURL: string, queryRequest: SiteRequest) {
        const signal = AbortSignal.timeout(3000);
        let response;
        try {
            response = await fetch(apiURL,
                {
                    method: "POST",
                    headers: {"Content-Type": "application/json"},
                    body: JSON.stringify(queryRequest),
                    signal: signal
                }
            );
    
            if(response.status == 200) {
                //SUCCESS
                let pageson: SiteResponse = await response.json();
                // process it into the view
                acceptNewDisplayItem(pageson);
            } else {
                //FAILED
                let pageson: SiteResponse = await response.json();
                if(pageson.Response) {
                    console.error("Site could not be loaded: " + pageson.Response);
                } else {
                    console.error("Site could not be loaded: The error message was also empty...");
                }
            }
        } catch (error) {
            console.error(error);
            //Set btn FAILED
            console.error("Site could not be loaded: We may have timed out. Or encounterd a more unexpected error.");
        }
        // Always re-enable the sending once we're done with the fetch
        props.sendItemRef.current.resultSend = true;
    }

    useEffect(() => {
        // Check siteID
        if(props.newitem.siteID < 0) {
            return;
        }
        // Validate incoming Variant
        let varSysLen = listVariantSYSTEM.length;
        let validVariant: boolean = false;
        for(let index = 0; index < varSysLen; index++) {
            if(listVariantSYSTEM[index].system == props.newitem.VariantSystem) {
                validVariant = true;
                break;
            }
        }

        if(validVariant) {
            // Trigger a fetch. The fetch will set the currently viewed site, and add it to the rotation mechanism
            fetchSite(APIURL, {VariantSystem: props.newitem.VariantSystem, SiteID: props.newitem.siteID});
        }
    },[props.newitem]);

    const [browserPassthrough, setBrowserPassthrough] = useState<listBrowserPassthrough>({toggle: false, sites: [], useVal: false });
    /* Browser Storage Functions */
    function prepareListForBrowser(){
        setBrowserPassthrough({toggle: !browserPassthrough.toggle, sites: listSites.current, useVal: true});
    }
    function replaceListFromBrowser(sites: SiteResponse[]) {
        listSites.current = sites;
        setSiteListPassthrough({toggle: !siteListPassthrough.toggle});
    }

    /* Toggle Open/Close extras */
    const [isExtras, setExtras] = useState<boolean>(false);
    function toggleExtras() {
        let newToggle = !isExtras;
        if(!newToggle) {
            setBrowserPassthrough({toggle: browserPassthrough.toggle, sites: browserPassthrough.sites, useVal: false});
        }
        setExtras(newToggle);
    }

    if(isExtras) {
        return (
            <>
            <div className="Utilities">
                <ItemViewUtiltyCoordinates parentDisplaySite={acceptNewDisplayItem}/>
                <UtilityOpenClose promptParentToggle={toggleExtras} />
                <ItemViewUtiltyFromBrowser parentPromptGetList={prepareListForBrowser}
                reportSiteList={replaceListFromBrowser} passthrough={browserPassthrough} />
            </div>
            <SiteList addSite={siteListPassthrough} informParentItemSelected={acceptNewDisplayItem} siteList={listSites} />
            {displayElement}
            </>
        );
    } else {
        return (
            <>
            <div className="Utilities">
                <UtilityOpenClose promptParentToggle={toggleExtras} />
            </div>
            <SiteList addSite={siteListPassthrough} informParentItemSelected={acceptNewDisplayItem} siteList={listSites} />
            {displayElement}
            </>
        );
    }
};