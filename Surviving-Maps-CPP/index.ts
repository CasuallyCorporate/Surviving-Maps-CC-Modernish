import express, {Express, Request, Response} from "express";
import bodyParser from "body-parser";
import fs, { readFileSync } from "fs";

import { Breakthrough, CoordinateObj, CoordRequest, listBreakthroughSYSTEM, listMapNames, listNamedLanding, listTopography, listVariantSYSTEM, PageRequest, PageResponse, PageSite, SiteRequest, SiteResponse } from './SM-Constants';

import TextCollection from './TextCollection';
import { setMaxIdleHTTPParsers } from "http";
import { execSync } from "child_process";

const app: Express = express();
const port: Number = 8000;

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json())

app.get("/", (req: Request, res: Response) => {
    res.send("Hello express + ts!");
});

const FAQHTML: TextCollection = new TextCollection(["<h1>FAQ</h1>"]);
interface faqitem {
    title: "";
    items: string[];
}
const ChangeHTML: TextCollection = new TextCollection(["<h1>Changelog</h1>"]);
interface CC_CL {
    version: "";
    items: string[];
}
interface Trickster_CL {
    date: "";
    items: string[];
}

function setupFAQHTML() {
    console.log("FAQ HTML");
    if(FAQHTML.isComplete()){return;}
    // open the json
    let faqitems: [faqitem];
    try {
        faqitems = JSON.parse(readFileSync(require.resolve('../FAQJson/mainFAQ.json'), 'utf8'));
    } catch (e) {
        console.log("Error in Current CC Json parse:", e);
        return;
    }
    // Insert into FAQHTML
    FAQHTML.addText("<h2>CasuallyCorporate/Surviving-Maps-CC-Modernish</h2>");
    faqitems.forEach(element => {
        FAQHTML.addText("<h3><u>Version: " + element.title + "</u></h3>");
        FAQHTML.addText("<ul>");
        element.items.forEach(thing => {
            FAQHTML.addText("<li>" + thing +"</li>");
        })
        FAQHTML.addText("</ul>");
    });

    FAQHTML.setComplete();
}

function setupChangeHTML() {
    if(ChangeHTML.isComplete()){return;}

    // open the json
    let Curritems: [CC_CL];
    try {
        Curritems = JSON.parse(readFileSync(require.resolve('../ChangelogJson/ChangeLog-Modernish.json'), 'utf8'));
    } catch (e) {
        console.log("Error in Current CC Json parse:", e);
        return;
    }
    // Insert into ChangeHTML
    ChangeHTML.addText("<h2>CasuallyCorporate/Surviving-Maps-CC-Modernish</h2>");
    Curritems.forEach(element => {
        ChangeHTML.addText("<h3><u>Version: " + element.version + "</u></h3>");
        ChangeHTML.addText("<ul>");
        element.items.forEach(thing => {
            ChangeHTML.addText("<li>" + thing +"</li>");
        })
        ChangeHTML.addText("</ul>");
    });
    ChangeHTML.addText("<hr/>");

    // open the json
    let CCitems: [CC_CL];
    try {
        CCitems = JSON.parse(readFileSync(require.resolve('../ChangelogJson/ChangeLog-CC.json'), 'utf8'));
    } catch (e) {
        console.log("Error in CC Json parse:", e);
        return;
    }
    // Insert into ChangeHTML
    ChangeHTML.addText("<h2>CasuallyCorporate/Surviving-Maps-CC</h2>");
    CCitems.forEach(element => {
        ChangeHTML.addText("<h3><u>Version: " + element.version + "</u></h3>");
        ChangeHTML.addText("<ul>");
        element.items.forEach(thing => {
            ChangeHTML.addText("<li>" + thing +"</li>");
        })
        ChangeHTML.addText("</ul>");
    });
    ChangeHTML.addText("<hr/>");

    // open the json
    let TRitems: [Trickster_CL];
    try {
        TRitems = JSON.parse(readFileSync(require.resolve('../ChangelogJson/ChangeLog-Trickster.json'), 'utf8'));
    } catch (e) {
        console.log("Error in Trickster Json parse:", e);
        return;
    }
    // Insert into ChangeHTML
    ChangeHTML.addText("<h2>trickster-is-weak/Surviving-Maps</h2>");
    TRitems.forEach(element => {
        ChangeHTML.addText("<h3><u>Date: " + element.date + "</u></h3>");
        ChangeHTML.addText("<ul>");
        element.items.forEach(thing => {
            ChangeHTML.addText("<li>" + thing +"</li>");
        })
        ChangeHTML.addText("</ul>");
    });

    ChangeHTML.setComplete();
}

interface motd_reqeustReceive{
    page: string;
}

app.post("/motd", bodyParser.json(), (req: Request, res: Response) => {
    if (typeof req.body == 'undefined') {
        res.setHeader("Content-Type", "application/json")
        .status(400)
        .send(JSON.stringify([
            {"Response": "Response body empty"}
        ]));
    }

    const reqjson: motd_reqeustReceive = req.body;
    console.log(reqjson.page);
    console.log(req.body);
    switch(reqjson.page)
    {
        case "Changelog":
            setupChangeHTML();
            if(!ChangeHTML.isComplete){ console.log("Failed to parse ChangeHTML json"); return;}
            res.setHeader("Content-Type", "application/json")
            .status(200)
            .send(JSON.stringify([
                {"Response": "Correct"},
                {"PageData": ChangeHTML.getText()}
            ]));
            break;
        case "FAQ":
            setupFAQHTML();
            if(!FAQHTML.isComplete){ console.log("Failed to parse FAQHTML json"); return;}
            res.setHeader("Content-Type", "application/json")
            .status(200)
            .send(JSON.stringify([
                {"Response": "Correct"},
                {"PageData": FAQHTML.getText()}
            ]));
            break;
        default:
            res.setHeader("Content-Type", "application/json")
            .status(501)
            .send(JSON.stringify([
                {"Response": "Nope"}
            ]));
            break;
    }

});

app.get("/motd", (req: Request, res: Response) => {
    res.setHeader("Content-Type", "application/json")
    .status(501)
    .send(JSON.stringify({"Response": "Not implemented"}));
});

function getErrorPageResponse(errortext: string): PageResponse {
    return {VariantSystem:"", PageNum:-1, PageTot: -1, PageSites:[], Response: errortext};
}

function returnPageSItes(amount: number): PageSite[] {
    let sites: PageSite[] = [];
    let sitesLen = amount;
    let tmpID = 1;
    for(let i = 0; i < sitesLen; i++ ) {
        let DustD: number = Math.floor(Math.random() * 4) + 1;
        let DustS: number = Math.floor(Math.random() * 4) + 1;
        let MetNum: number = Math.floor(Math.random() * 4) + 1;
        let ColdW: number = Math.floor(Math.random() * 4) + 1;
        let DisTot:number = DustD + DustS + MetNum + ColdW;

        let Conc: number = Math.floor(Math.random() * 4) + 1;
        let Wat: number = Math.floor(Math.random() * 4) + 1;
        let Met: number = Math.floor(Math.random() * 4) + 1;
        let RMet: number = Math.floor(Math.random() * 4) + 1;
        let ResTot: number = Conc + Wat + Met + RMet;

        let tmpCoord: CoordinateObj = getRandomCoordinate();
        let strCoord: string = tmpCoord.NSNum + tmpCoord.NSChar + ":" + tmpCoord.EWNum + tmpCoord.EWChar;

        sites.push({ID: tmpID, Coordinates: strCoord, DisastersTot: DisTot, ResourcesTot: ResTot,
            DustDevils: DustD, DustStorms: DustS, Meteors: MetNum, ColdWaves: ColdW,
            Concrete: Conc, Water: Wat, Metal: Met, RareMetal: RMet
         });
        tmpID++;
    }

    return sites;
}

// Use PageRequest from SM-Constants
app.post("/page", bodyParser.json(), (req: Request, res: Response) => {
    if (typeof req.body == 'undefined') {
        res.setHeader("Content-Type", "application/json")
        .status(400)
        .send(getErrorPageResponse("400: Bad Request"));
        return;
    }

    const reqjson: PageRequest = req.body;

    // Do not trust at any point that the received request meets the interface(s) spec
    // Always check for existence

    console.log("/page");
    if(reqjson.Simple) {
        console.log("Simple exists");
        if(reqjson.Simple.Variant){
            console.log("Variant " + reqjson.Simple.Variant);
            // Must have a variant
            switch(reqjson.Simple.Variant) {
                // This is here as a reminder that the permanent server will need to respond to variants differently
                default:
                break;
            }
            let pageItemMax = 20;
            let pageToReturn = 1;
            let pageTotal = 40;
            if(reqjson.PageNum) {
                pageToReturn = reqjson.PageNum;
                console.log("Included request for page: " + pageToReturn);
            }
            console.log("Returning page: " + pageToReturn);
            if(!(pageToReturn > pageTotal)){
                let tmpPageSuccessResponse: PageResponse = {VariantSystem: reqjson.Simple.Variant, PageNum: pageToReturn, PageTot: pageTotal, PageSites: []};
                // Randomly generate for now to test
                tmpPageSuccessResponse.PageSites = returnPageSItes(pageItemMax);
                res.setHeader("Content-Type", "application/json")
                .status(200)
                .send(tmpPageSuccessResponse);
                return;
            }
            // Fall through on error
        }
    }

    // Fall through to bad request. Presume the request itself is bad
    res.setHeader("Content-Type", "application/json")
    .status(400)
    .send(getErrorPageResponse("400: Bad Request"));
});

app.get("/page", (req: Request, res: Response) => {
    res.setHeader("Content-Type", "application/json")
    .status(501)
    .send(JSON.stringify({"Response": "Not implemented"}));
});

function getErrorSiteResponse(errortext: string): SiteResponse {
    return({Response: errortext, VariantSystem: "", SiteID: -1,
        Coordinate: {NSNum: -1, NSChar:"N", EWNum: -1, EWChar:"W"},
    
        NamedLandingSYS: "", TopographySYS: "", MapName: "",
    
        Challenge: -1, Altitude: -1, Temperature: -1,
    
        DustDevils: -1, DustStorms: -1, Meteors: -1, ColdWaves: -1,
    
        Concrete: -1, Water: -1, Metal: -1, RareMetal: -1,
    
        MissingBreakthroughs: -1, Breakthroughs: []});
}

function getRandomDisResNum(): number {
    return Math.floor(Math.random() * 4) + 1;
}
function getRandomCoordinate(): CoordinateObj {
    let tmpCoord: CoordinateObj = {NSNum: -1, NSChar:"S", EWNum: -1, EWChar:"E"};

    tmpCoord.NSNum = Math.floor(Math.random() * 71); // 0 - 70
    tmpCoord.EWNum = Math.floor(Math.random() * 181); // 0 - 180

    let tmpDecider = Math.floor(Math.random() * 2); // 0 - 1
    if(tmpDecider == 0) {
        tmpCoord.NSChar="N";
    } else {
        tmpCoord.NSChar="S";
    }

    tmpDecider = Math.floor(Math.random() * 2); // 0 - 1
    if(tmpDecider == 0) {
        tmpCoord.EWChar="E";
    } else {
        tmpCoord.EWChar="W";
    }

    return tmpCoord;
}
function getRandomLanding(): string {
    let Len = listNamedLanding.length;
    let index = Math.floor(Math.random() * Len);
    return listNamedLanding[index].system;
}
function getRandomTopography(): string {
    let Len = listTopography.length;
    let index = Math.floor(Math.random() * Len);
    return listTopography[index].system;
}
function getRandomMapName(): string {
    let Len = listMapNames.length;
    let index = Math.floor(Math.random() * Len);
    return listMapNames[index].system;
}
function getRandomBreakthroughs(): string[] {
    let tmpBtrs: string[] = [];
    let tmpTMPBTR: string = listBreakthroughSYSTEM[0];
    let numBtrsLen = listBreakthroughSYSTEM.length;
    let index = -1;

    while(tmpBtrs.length < 13) {
        index = Math.floor(Math.random() * numBtrsLen );
        tmpTMPBTR = listBreakthroughSYSTEM[index];
        if(tmpBtrs.indexOf(tmpTMPBTR) == -1) {
            tmpBtrs.push(tmpTMPBTR);
        } else {
            continue;
        }
    }
    return tmpBtrs;
}

app.post("/site", bodyParser.json(), (req: Request, res: Response) => {
    if (typeof req.body == 'undefined') {
        res.setHeader("Content-Type", "application/json")
        .status(400)
        .send(getErrorSiteResponse("400: Bad Request"));
        return;
    }

    const reqjson: SiteRequest = req.body;

    console.log("/site");
    let validVariant: boolean = false;
    if(reqjson.VariantSystem) {
        // Validate incoming Variant
        let varSysLen = listVariantSYSTEM.length;
        
        for(let index = 0; index < varSysLen; index++) {
            if(listVariantSYSTEM[index].system == reqjson.VariantSystem) {
                validVariant = true;
                break;
            }
        }
    }

    if(validVariant && reqjson.SiteID >= 0) {
        console.log("Variant valid, and siteID >= 0");
        let tmpSiteResponse: SiteResponse = {VariantSystem: reqjson.VariantSystem, SiteID: reqjson.SiteID,
            Coordinate: getRandomCoordinate(),
        
            NamedLandingSYS: getRandomLanding(), TopographySYS: getRandomTopography(), MapName: getRandomMapName(),
        
            Challenge: 0, Altitude: -1, Temperature: -1,
        
            DustDevils: getRandomDisResNum(), DustStorms: getRandomDisResNum(), Meteors: getRandomDisResNum(), ColdWaves: getRandomDisResNum(),
        
            Concrete: getRandomDisResNum(), Water: getRandomDisResNum(), Metal: getRandomDisResNum(), RareMetal: getRandomDisResNum(),
        
            MissingBreakthroughs: 0, Breakthroughs: getRandomBreakthroughs()
        };
        
        res.setHeader("Content-Type", "application/json")
        .status(200)
        .send(tmpSiteResponse);
        return;
    }

    // Fall through to bad request. Presume the request itself is bad
    res.setHeader("Content-Type", "application/json")
    .status(400)
    .send(getErrorSiteResponse("400: Bad Request"));
});

app.get("/site", (req: Request, res: Response) => {
    res.setHeader("Content-Type", "application/json")
    .status(501)
    .send(JSON.stringify({"Response": "Not implemented"}));
});

app.post("/coord", bodyParser.json(), (req: Request, res: Response) => {
    if (typeof req.body == 'undefined') {
        res.setHeader("Content-Type", "application/json")
        .status(400)
        .send(getErrorSiteResponse("400: Bad Request"));
        return;
    }

    const reqjson: CoordRequest = req.body;

    // Validate the variant
    let validVariant: boolean = false;
    if(reqjson.VariantSystem) {
        // Validate incoming Variant
        let varSysLen = listVariantSYSTEM.length;
        
        for(let index = 0; index < varSysLen; index++) {
            if(listVariantSYSTEM[index].system == reqjson.VariantSystem) {
                validVariant = true;
                break;
            }
        }
    }

    if(validVariant) {
        // Validate coordinate IRL, but here we're just going to randomly throw an error
        let randFail: number = Math.floor(Math.random() * 100);
        if(randFail > 60 && randFail < 64) {
            switch(randFail) {
                case 61: // Coordinate is wrong format
                    res.setHeader("Content-Type", "application/json")
                    .status(400)
                    .send(getErrorSiteResponse("400: Coordinate is malformed"));
                    return;
                    break;
                case 62: // Coordinate does not exist
                    res.setHeader("Content-Type", "application/json")
                    .status(400)
                    .send(getErrorSiteResponse("400: Coordinate does not exist"));
                return;
                    break;
                default:
                    res.setHeader("Content-Type", "application/json")
                    .status(400)
                    .send(getErrorSiteResponse("400: Bad Request"));
                    return;
            }
        }

        // Generate a SiteResponse
        let tmpSiteResponse: SiteResponse = {VariantSystem: reqjson.VariantSystem, SiteID: Math.floor(Math.random()*1000),
            Coordinate: getRandomCoordinate(),
        
            NamedLandingSYS: getRandomLanding(), TopographySYS: getRandomTopography(), MapName: getRandomMapName(),
        
            Challenge: 0, Altitude: -1, Temperature: -1,
        
            DustDevils: getRandomDisResNum(), DustStorms: getRandomDisResNum(), Meteors: getRandomDisResNum(), ColdWaves: getRandomDisResNum(),
        
            Concrete: getRandomDisResNum(), Water: getRandomDisResNum(), Metal: getRandomDisResNum(), RareMetal: getRandomDisResNum(),
        
            MissingBreakthroughs: 0, Breakthroughs: getRandomBreakthroughs()
        };
        
        res.setHeader("Content-Type", "application/json")
        .status(200)
        .send(tmpSiteResponse);
        return;
    }

    // Fall through to bad request. Presume the request itself is bad
    res.setHeader("Content-Type", "application/json")
    .status(400)
    .send(getErrorSiteResponse("400: Bad Request"));
});

app.get("/coord", (req: Request, res: Response) => {
    res.setHeader("Content-Type", "application/json")
    .status(501)
    .send(JSON.stringify({"Response": "Not implemented"}));
});

app.listen(port, () => {
    console.log('now listening');
});