export default class TextCollection {
    private _always: string[] = [];
    private _texts: string[] = [];
    private _complete: string = "";

    private _isComplete = false;

    public constructor(texts: string[]) {
        this._always = texts;
    }
    public addText(text: string) {
        if(this._isComplete){return;}
        this._texts.push(text);
    }
    public addTexts(texts: string[]) {
        if(this._isComplete){return;}
        this._texts = this._texts.concat(texts);
    }

    public setComplete() {
        this._isComplete = true;
    }
    public isComplete(): boolean {
        return this._isComplete;
    }

    public getText(): string {
        if(this._complete) {
            if(this._complete == "") {
                this._complete = this._always.concat(this._texts).join("");
            }
            return this._complete;
        }
        else {
            return (this._always.concat(this._texts).join(""));
        }
    }
}