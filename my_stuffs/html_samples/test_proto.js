
function createPluginReplacement()
{
    return new Replacement();
};

function Replacement()
{
    this.one = 1;
    this.scriptObject = {};

    this.createScriptInterface();
};

Replacement.prototype = {
    MethodMap: {
        getSth : 'getSth'
    },
 
    createScriptInterface: function()
    {
        for (name in this.MethodMap) {
            var methodName = this.MethodMap[name];
            this.scriptObject[name] = this[methodName].bind(this);
        }
    },

    getSth: function() {
        return 1337;
    }
};