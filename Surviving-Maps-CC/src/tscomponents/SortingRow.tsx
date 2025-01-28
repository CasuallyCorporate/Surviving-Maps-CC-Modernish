import { MutableRefObject, useRef, useState } from "react";
import { SortingEnabled } from "./appMain";

interface itemProps {
    className: string;
    itemName: String;
    toggled(itemName: String): any;
    changedSelection(): any;
}

const SortingItem = (props: itemProps) => {

    let addedIcon = "";

    switch(props.itemName){
        case "Dust Devils":
            addedIcon = "icon-devils";
            break;
        case "Dust Storms":
            addedIcon = "icon-storms";
            break;
        case "Meteors":
            addedIcon = "icon-meteors";
            break;
        case "Cold Waves":
            addedIcon = "icon-cold";
            break;

        case "Concrete":
            addedIcon = "icon-concrete";
            break;
        case "Water":
            addedIcon = "icon-water";
            break;
        case "Metal":
            addedIcon = "icon-metal";
            break;
        case "Rare Metal":
            addedIcon = "icon-raremetal";
            break;
    }

    function localToggle(){
        props.toggled(props.itemName);
        props.changedSelection();
    }

    if(addedIcon != "") {
        return(
            <>
            <a className={props.className + " stickDown"} onClick={localToggle}>
            {props.itemName}
            <div className={"icon-resource " + addedIcon}></div>
            <div className="Chevron"></div>
            </a>
            </>
        );
    } else {
        return(
            <>
            <a className={props.className} onClick={localToggle}>
            {props.itemName}
            <div className="Chevron"></div>
            </a>
            </>
        );
    }
}

interface sortingProps {
    returnSortingEnabled: MutableRefObject<SortingEnabled>;
    changedSelection(): any;
}

export const SortingRow= (props: sortingProps) => {

    function getItemIndex (itemName: string): number {
        switch(itemName){
            case "Location":
                return 0;
                break;
            case "Disasters":
                return 1;
                break;
            case "Resources":
                return 2;
                break;

            case "Dust Devils":
                return 3;
                break;
            case "Dust Storms":
                return 4;
                break;
            case "Meteors":
                return 5;
                break;
            case "Cold Waves":
                return 6;
                break;
    
            case "Concrete":
                return 7;
                break;
            case "Water":
                return 8;
                break;
            case "Metal":
                return 9;
                break;
            case "Rare Metal":
                return 10;
                break;
        }
        //Fall through
        return -1;
    }

    const firstRun: MutableRefObject<boolean> = useRef(true);
    const itemClassArray: MutableRefObject<string[]> = useRef(new Array(11));
    const selecteditem: MutableRefObject<number> = useRef(-1);
    const [currentSortReq, setSortReq] = useState({isLowest: false, SortItem: ""});

    if(firstRun.current) {
        itemClassArray.current.fill("col-md-1");
        firstRun.current = false;
    }
    
    function subTitleToggled(itemName: string) {

        let itemIndex = getItemIndex(itemName);

        if(itemIndex == -1) {
            console.error("SortingRow: ItemName index returned -1");
            return;
        }

        if(props.returnSortingEnabled.current.isEnabled) {
            // Something is selected
            if(currentSortReq.SortItem == itemName ){
                // The item being toggled is the current item.
                // Pass through these states: Highest, Lowest, Off

                if(currentSortReq.isLowest){
                    // Move to off
                    props.returnSortingEnabled.current.isEnabled = false;
                    itemClassArray.current[selecteditem.current] = "col-md-1";
                    selecteditem.current = -1;
                    // Prompt re-draw
                    setSortReq({isLowest: false, SortItem: ""});
                } else {
                    // isHighest. Move to isLowest
                    setSortReq({
                        ...currentSortReq,
                        isLowest: true
                    });
                    props.returnSortingEnabled.current.sortingReq = {
                        ...currentSortReq,
                        isLowest: true
                    };
                    itemClassArray.current[itemIndex] = "col-md-1 SelectedDown";
                }
                return;
            }
            // Else fall through (Not the current item)
        }
        // not currently enabled, or fall-through.
        // First undo any previous selection
        if(selecteditem.current != -1){
            itemClassArray.current[selecteditem.current] = "col-md-1";
        }
        // then set item as current + isHighest
        setSortReq({isLowest: false, SortItem: itemName});
        // Set it to enabled
        props.returnSortingEnabled.current.isEnabled = true;
        props.returnSortingEnabled.current.sortingReq = {isLowest: false, SortItem: itemName};
        // Set selectedItem
        selecteditem.current = itemIndex;
        itemClassArray.current[selecteditem.current] = "col-md-1 SelectedUp";
    }

    return(
        <>
        <SortingItem className={itemClassArray.current[0]} itemName={"Location"} toggled={subTitleToggled} changedSelection={props.changedSelection} />
        <SortingItem className={itemClassArray.current[1]} itemName={"Disasters"} toggled={subTitleToggled} changedSelection={props.changedSelection} />
        <SortingItem className={itemClassArray.current[2]} itemName={"Resources"} toggled={subTitleToggled} changedSelection={props.changedSelection} />

        <SortingItem className={itemClassArray.current[3]} itemName={"Dust Devils"} toggled={subTitleToggled} changedSelection={props.changedSelection} />
        <SortingItem className={itemClassArray.current[4]} itemName={"Dust Storms"} toggled={subTitleToggled} changedSelection={props.changedSelection} />
        <SortingItem className={itemClassArray.current[5]} itemName={"Meteors"} toggled={subTitleToggled} changedSelection={props.changedSelection} />
        <SortingItem className={itemClassArray.current[6]} itemName={"Cold Waves"} toggled={subTitleToggled} changedSelection={props.changedSelection} />

        <SortingItem className={itemClassArray.current[7]} itemName={"Concrete"} toggled={subTitleToggled} changedSelection={props.changedSelection} />
        <SortingItem className={itemClassArray.current[8]} itemName={"Water"} toggled={subTitleToggled} changedSelection={props.changedSelection} />
        <SortingItem className={itemClassArray.current[9]} itemName={"Metal"} toggled={subTitleToggled} changedSelection={props.changedSelection} />
        <SortingItem className={itemClassArray.current[10]} itemName={"Rare Metal"} toggled={subTitleToggled} changedSelection={props.changedSelection} />
        </>
    );
}