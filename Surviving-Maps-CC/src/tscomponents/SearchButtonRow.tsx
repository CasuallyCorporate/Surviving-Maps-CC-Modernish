import { MutableRefObject, useEffect, useRef, useState } from "react";
import { PageRequest, PageResponse, ButtonState } from "./SM-Constants";


export interface changesStateObj {
    toggle: boolean;
    changed: boolean;
}

interface ButtonRowProps {
    getFullRequest(): PageRequest;
    setLoadingAnimation(setLoading: boolean): any;
    returnsResults(response: PageResponse): any;
    specificPage: number;
    changedSearchValues: changesStateObj;
}

export const SearchButtonRow = (props: ButtonRowProps) => {
    const APIURL: string = "/page";

    const [buttonState, setButtonState] = useState<ButtonState>(ButtonState.IDLE);
    const [ToolTip, setTooltTip] = useState("");
    const currentlySearching: MutableRefObject<boolean> = useRef(false);
    const lastSearch: MutableRefObject<PageRequest> = useRef({Simple:{}});

    /* Fetch and timeout stuff */
    async function fetchResults(apiURL: string, queryRequest: PageRequest) {
        props.setLoadingAnimation(true);
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
                //Set btn SUCCESS
                setButtonState(ButtonState.SUCCESS);
                let pageson: PageResponse = await response.json();
                props.returnsResults(pageson);
                setTooltTip("");
            } else {
                //Set btn FAILED
                setButtonState(ButtonState.FAILED);
                let pageson: PageResponse = await response.json();
                props.returnsResults(pageson);
                if(pageson.Response) {
                    setTooltTip(pageson.Response);
                } else {
                    setTooltTip("Error, but the server did not send a specific error...");
                }
                
            }
        } catch (error) {
            console.error(error);
            //Set btn FAILED
            props.returnsResults({VariantSystem: "", PageNum: -1, PageTot: -1, PageSites: []});
            setButtonState(ButtonState.FAILED);
            setTooltTip("Server unavailable, but may have timed-out"); // TODO: Confirm
        }

        // Put re-enable the button
        currentlySearching.current = false;
        props.setLoadingAnimation(false);
    }

    function getAndSend(pageNum: number) {
        if(currentlySearching.current){
            return;
        }
        // Disable the button
        //Set btn disabled
        currentlySearching.current = true;
        setButtonState(ButtonState.SEARCHING);
        // get the request
        let request: PageRequest = props.getFullRequest();
        // Are we requesting a specific page?
        if(pageNum > 0){
            request.PageNum = pageNum;
        }
        // Check we're not re-sending
        if(request != lastSearch.current) {
            // Not equal, searching!
            // Send the request
            fetchResults(APIURL, request);
            // Set the last request to this one
            lastSearch.current = request;
        } else {
            // Not searching
            // Put re-enable the button
            currentlySearching.current = false;
            props.setLoadingAnimation(false);
            return;
        }
    }

    function localSend(){
        getAndSend(props.specificPage);
    }

    useEffect(() => {
        // For changing the button in the condition where the page number being requested changes
        if(props.specificPage > 0){
            getAndSend(props.specificPage);
        }
    }
    , [props.specificPage]);

    useEffect(() => {
        if(props.changedSearchValues.changed){
            if(buttonState != ButtonState.SEARCHING && buttonState != ButtonState.FAILED) {
                //Setting to changed
                setButtonState(ButtonState.CHANGED);
                setTooltTip("Parameters changed. Press search for new results!");
            }
        } else {
            if(buttonState == ButtonState.CHANGED) {
                //Setting tooltip to empty
                setButtonState(ButtonState.IDLE);
                setTooltTip("");
            }
        }
    }
    , [props.changedSearchValues]);

    if(ToolTip == "") {
        return(
            <>
                <button type="button" className={"btn btn-lg" +" "+ buttonState} onClick={localSend}>Search</button>
            </>
        );
    } else {
        return(
            <>
                <button type="button" className={"btn btn-lg" +" "+ buttonState} onClick={localSend}>Search</button>
                <div className="hover-tooltip">{ToolTip}</div>
            </>
        );
    }
}