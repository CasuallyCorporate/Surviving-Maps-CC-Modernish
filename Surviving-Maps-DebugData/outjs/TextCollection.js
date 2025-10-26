"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class TextCollection {
    constructor(texts) {
        this._always = [];
        this._texts = [];
        this._complete = "";
        this._isComplete = false;
        this._always = texts;
    }
    addText(text) {
        if (this._isComplete) {
            return;
        }
        this._texts.push(text);
    }
    addTexts(texts) {
        if (this._isComplete) {
            return;
        }
        this._texts = this._texts.concat(texts);
    }
    setComplete() {
        this._isComplete = true;
    }
    isComplete() {
        return this._isComplete;
    }
    getText() {
        if (this._complete) {
            if (this._complete == "") {
                this._complete = this._always.concat(this._texts).join("");
            }
            return this._complete;
        }
        else {
            return (this._always.concat(this._texts).join(""));
        }
    }
}
exports.default = TextCollection;
