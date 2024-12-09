import express, {Express, Request, Response} from "express";
import bodyParser from "body-parser";
import fs, { readFileSync } from "fs";

import TextCollection from './TextCollection';

const app: Express = express();
const port: Number = 8000;

interface reqeustReceive{
    page: string;
}

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json())

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

app.get("/", (req: Request, res: Response) => {
    res.send("Hello express + ts!");
});

app.post("/endpoint", bodyParser.json(), (req: Request, res: Response) => {
    if (typeof req.body == 'undefined') {
        res.setHeader("Content-Type", "application/json")
        .status(400)
        .send(JSON.stringify([
            {"Response": "Response body empty"}
        ]));
    }

    const reqjson: reqeustReceive = req.body;
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

app.get("/endpoint", (req: Request, res: Response) => {
    res.setHeader("Content-Type", "application/json")
    .status(501)
    .send(JSON.stringify({"Response": "Not implemented"}));
});

app.listen(port, () => {
    console.log('now listening');
});