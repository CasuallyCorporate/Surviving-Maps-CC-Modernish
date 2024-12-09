"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const body_parser_1 = __importDefault(require("body-parser"));
const fs_1 = require("fs");
const TextCollection_1 = __importDefault(require("./TextCollection"));
const app = (0, express_1.default)();
const port = 8000;
app.use(body_parser_1.default.urlencoded({ extended: true }));
app.use(body_parser_1.default.json());
const FAQHTML = new TextCollection_1.default(["<h1>FAQ</h1>"]);
const ChangeHTML = new TextCollection_1.default(["<h1>Changelog</h1>"]);
function setupFAQHTML() {
    console.log("FAQ HTML");
    if (FAQHTML.isComplete()) {
        return;
    }
    // open the json
    let faqitems;
    try {
        faqitems = JSON.parse((0, fs_1.readFileSync)(require.resolve('../FAQJson/mainFAQ.json'), 'utf8'));
    }
    catch (e) {
        console.log("Error in Current CC Json parse:", e);
        return;
    }
    // Insert into FAQHTML
    FAQHTML.addText("<h2>CasuallyCorporate/Surviving-Maps-CC-Modernish</h2>");
    faqitems.forEach(element => {
        FAQHTML.addText("<h3><u>Version: " + element.title + "</u></h3>");
        FAQHTML.addText("<ul>");
        element.items.forEach(thing => {
            FAQHTML.addText("<li>" + thing + "</li>");
        });
        FAQHTML.addText("</ul>");
    });
    FAQHTML.setComplete();
}
function setupChangeHTML() {
    if (ChangeHTML.isComplete()) {
        return;
    }
    // open the json
    let Curritems;
    try {
        Curritems = JSON.parse((0, fs_1.readFileSync)(require.resolve('../ChangelogJson/ChangeLog-Modernish.json'), 'utf8'));
    }
    catch (e) {
        console.log("Error in Current CC Json parse:", e);
        return;
    }
    // Insert into ChangeHTML
    ChangeHTML.addText("<h2>CasuallyCorporate/Surviving-Maps-CC-Modernish</h2>");
    Curritems.forEach(element => {
        ChangeHTML.addText("<h3><u>Version: " + element.version + "</u></h3>");
        ChangeHTML.addText("<ul>");
        element.items.forEach(thing => {
            ChangeHTML.addText("<li>" + thing + "</li>");
        });
        ChangeHTML.addText("</ul>");
    });
    ChangeHTML.addText("<hr/>");
    // open the json
    let CCitems;
    try {
        CCitems = JSON.parse((0, fs_1.readFileSync)(require.resolve('../ChangelogJson/ChangeLog-CC.json'), 'utf8'));
    }
    catch (e) {
        console.log("Error in CC Json parse:", e);
        return;
    }
    // Insert into ChangeHTML
    ChangeHTML.addText("<h2>CasuallyCorporate/Surviving-Maps-CC</h2>");
    CCitems.forEach(element => {
        ChangeHTML.addText("<h3><u>Version: " + element.version + "</u></h3>");
        ChangeHTML.addText("<ul>");
        element.items.forEach(thing => {
            ChangeHTML.addText("<li>" + thing + "</li>");
        });
        ChangeHTML.addText("</ul>");
    });
    ChangeHTML.addText("<hr/>");
    // open the json
    let TRitems;
    try {
        TRitems = JSON.parse((0, fs_1.readFileSync)(require.resolve('../ChangelogJson/ChangeLog-Trickster.json'), 'utf8'));
    }
    catch (e) {
        console.log("Error in Trickster Json parse:", e);
        return;
    }
    // Insert into ChangeHTML
    ChangeHTML.addText("<h2>trickster-is-weak/Surviving-Maps</h2>");
    TRitems.forEach(element => {
        ChangeHTML.addText("<h3><u>Date: " + element.date + "</u></h3>");
        ChangeHTML.addText("<ul>");
        element.items.forEach(thing => {
            ChangeHTML.addText("<li>" + thing + "</li>");
        });
        ChangeHTML.addText("</ul>");
    });
    ChangeHTML.setComplete();
}
app.get("/", (req, res) => {
    res.send("Hello express + ts!");
});
app.post("/endpoint", body_parser_1.default.json(), (req, res) => {
    if (typeof req.body == 'undefined') {
        res.setHeader("Content-Type", "application/json")
            .status(400)
            .send(JSON.stringify([
            { "Response": "Response body empty" }
        ]));
    }
    const reqjson = req.body;
    console.log(reqjson.page);
    console.log(req.body);
    switch (reqjson.page) {
        case "Changelog":
            setupChangeHTML();
            if (!ChangeHTML.isComplete) {
                console.log("Failed to parse ChangeHTML json");
                return;
            }
            res.setHeader("Content-Type", "application/json")
                .status(200)
                .send(JSON.stringify([
                { "Response": "Correct" },
                { "PageData": ChangeHTML.getText() }
            ]));
            break;
        case "FAQ":
            setupFAQHTML();
            if (!FAQHTML.isComplete) {
                console.log("Failed to parse FAQHTML json");
                return;
            }
            res.setHeader("Content-Type", "application/json")
                .status(200)
                .send(JSON.stringify([
                { "Response": "Correct" },
                { "PageData": FAQHTML.getText() }
            ]));
            break;
        default:
            res.setHeader("Content-Type", "application/json")
                .status(501)
                .send(JSON.stringify([
                { "Response": "Nope" }
            ]));
            break;
    }
});
app.get("/endpoint", (req, res) => {
    res.setHeader("Content-Type", "application/json")
        .status(501)
        .send(JSON.stringify({ "Response": "Not implemented" }));
});
app.listen(port, () => {
    console.log('now listening');
});
