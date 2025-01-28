import { MutableRefObject, useRef, FormEvent, useState, useEffect } from "react";
import { DropCoordinateVariant } from "./Dropdowns";
import { CoordinateObj, CoordRequest, SiteResponse, ButtonState } from "./SM-Constants";

export interface CoordinateProps {
    parentDisplaySite(site: SiteResponse): any;
}
export const ItemViewUtiltyCoordinates = (props: CoordinateProps) => {

    const apiURL: string = "/coord";
    const [buttonState, setButtonState] = useState<ButtonState>(ButtonState.IDLE);
    const [ToolTip, setTooltTip] = useState("");

    const variantChosen: MutableRefObject<string> = useRef("STANDARD");

    async function fetchwithCoord(coordinate: CoordinateObj) {
        setButtonState(ButtonState.SEARCHING);
        setTooltTip("");
        let request: CoordRequest = {VariantSystem: variantChosen.current, Coord: coordinate};
        const signal = AbortSignal.timeout(3000);
        let response;
        try {
            response = await fetch(apiURL,
                {
                    method: "POST",
                    headers: {"Content-Type": "application/json"},
                    body: JSON.stringify(request),
                    signal: signal
                }
            );
        
            if(response.status == 200) {
                //Set btn SUCCESS
                setButtonState(ButtonState.SUCCESS);
                let pageson: SiteResponse = await response.json();
                props.parentDisplaySite(pageson);
                setTooltTip("");
            } else {
                //Set btn FAILED
                setButtonState(ButtonState.FAILED);
                let pageson: SiteResponse = await response.json();
                if(pageson.Response) {
                    setTooltTip(pageson.Response);
                } else {
                    setTooltTip("Error, but the server did not send a specific error...");
                }
            }
        } catch (error) {
            console.error(error);
            //Set btn FAILED
            setButtonState(ButtonState.FAILED);
            setTooltTip("Server unavailable, but may have timed-out"); // TODO: Confirm
        }
    }

    function errorParseForm(){
        // Change button to error
        setButtonState(ButtonState.FAILED);
        setTooltTip("Coordinate could not be Parsed");
        console.error("Coordinate Parse");
    }

    function dealWithForm(e: FormEvent<HTMLFormElement>) {
        e.preventDefault();

        if(buttonState == ButtonState.SEARCHING) {
            return;
        }

        const formData = new FormData(e.currentTarget);
        let testJSON: CoordinateObj = {NSNum: 90, NSChar:"N", EWNum: 80, EWChar:"E"};
        let formJSON = Object.fromEntries(formData.entries());

        // convert form formJSON to testJSON
        let parseError: boolean = false;
        if(formJSON.NSChar) {
            if(formJSON.NSChar == "N" || formJSON.NSChar == "S") {
                testJSON.NSChar = formJSON.NSChar;
            } else { parseError = true;}
        } else { parseError = true;}

        if(!parseError && formJSON.EWChar) {
            if(formJSON.EWChar == "E" || formJSON.EWChar == "W") {
                testJSON.EWChar = formJSON.EWChar;
            } else { parseError = true;}
        } else { parseError = true;}

        if(!parseError && formJSON.NSNum) {
            testJSON.NSNum = Number(formJSON.NSNum);
            if(Number.isNaN(testJSON.NSNum)) {
                parseError = true;
            }
        } else { parseError = true;}

        if(formJSON.EWNum) {
            testJSON.EWNum = Number(formJSON.EWNum);
            if(Number.isNaN(testJSON.EWNum)) {
                parseError = true;
            }
        } else { parseError = true;}

        if(parseError) {
            errorParseForm();
        } else {
            fetchwithCoord(testJSON);
        }
    }

    function variantChanged() {
        console.log(variantChosen.current);
    }

    if(ToolTip == "") {
        return (
            <>
            <div className="addCoord Utility">
                <DropCoordinateVariant selectionChanged={variantChanged} returnCoordinateRef={variantChosen} />
                <form method="post" className="CoordInput" onSubmit={dealWithForm}>
                    <div className="Dirs">
                        <div className="direction">
                            <input name="NSNum" type="number" defaultValue={0} />
                            <p className="Radio">
                                <label>
                                    <input type="radio" name="NSChar" value="N" />
                                    N
                                </label>
                                <label>
                                    <input type="radio" name="NSChar" value="S" />
                                    S
                                </label>
                            </p>
                        </div>
                        <div className="direction">
                            <input name="EWNum" type="number" defaultValue={0} />
                            <p className="Radio">
                                <label>
                                    <input type="radio" name="EWChar" value="E" />
                                    E
                                </label>
                                <label>
                                    <input type="radio" name="EWChar" value="W" />
                                    W
                                </label>
                            </p>
                        </div>
                    </div>
                    <div className="tool-tipped">
                        <button type="submit" className={"btn " + buttonState}>Add from Coordinate</button>
                    </div>
                </form>
            </div>
            </>
        );
    } else {
        return (
            <>
            <div className="addCoord Utility">
                <DropCoordinateVariant selectionChanged={variantChanged} returnCoordinateRef={variantChosen} />
                <form method="post" className="CoordInput" onSubmit={dealWithForm}>
                    <div className="Dirs">
                        <div className="direction">
                            <input name="NSNum" type="number" defaultValue={0} />
                            <p className="Radio">
                                <label>
                                    <input type="radio" name="NSChar" value="N" />
                                    N
                                </label>
                                <label>
                                    <input type="radio" name="NSChar" value="S" />
                                    S
                                </label>
                            </p>
                        </div>
                        <div className="direction">
                            <input name="EWNum" type="number" defaultValue={0} />
                            <p className="Radio">
                                <label>
                                    <input type="radio" name="EWChar" value="E" />
                                    E
                                </label>
                                <label>
                                    <input type="radio" name="EWChar" value="W" />
                                    W
                                </label>
                            </p>
                        </div>
                    </div>
                    <div className="tool-tipped">
                        <button type="submit" className={"btn " + buttonState}>Add from Coordinate</button>
                        <div className="hover-tooltip">{ToolTip}</div>
                    </div>
                </form>
            </div>
            </>
        );
    }
}

export interface listBrowserPassthrough {
    toggle: boolean;
    sites: SiteResponse[];
    useVal: boolean;
}
export interface ToFromBrowserProps {
    parentPromptGetList(): any;
    reportSiteList(sites: SiteResponse[]): any;
    passthrough: listBrowserPassthrough;
}
export const ItemViewUtiltyFromBrowser = (props: ToFromBrowserProps) => {

    function saveToBrowser() {
        // Prompt parent to give the currentList in useEffect
        props.parentPromptGetList();
    }

    function loadFromBrowser() {
        // Get directly from browser
        let tmpStrSites: string | null = localStorage.getItem('SurvivingMars-CC');
        if (tmpStrSites) {
            // Convert
            let tmpSites: SiteResponse[] = JSON.parse(tmpStrSites);
            // Report it to the parent
            props.reportSiteList(tmpSites);
        } else {
            console.log("No string retuned from the browser");
        }
    }

    // get items and save to browser
    useEffect(() => {
        if(!props.passthrough.useVal) { return;}
        localStorage.setItem('SurvivingMars-CC',JSON.stringify(props.passthrough.sites));
    },[props.passthrough]);

    return (
        <>
        <div className="ListFromBrowser Utility">
            <button className="btn btn-light" onClick={loadFromBrowser}>Load list {'<-'} Browser</button>
            <button className="btn btn-light" onClick={saveToBrowser}>Save list {'->'} Browser</button>
        </div>
        </>
    );
}

export interface UtilityOpenCloseProps {
    promptParentToggle(): any;
}
export const UtilityOpenClose = (props: UtilityOpenCloseProps) => {
    return(
        <>
        <div className="UtilityToggle">
            <a onClick={props.promptParentToggle}>Coordinates & Save/Load</a>
        </div>
        </>
    );
}