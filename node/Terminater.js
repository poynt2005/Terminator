var path = require("path");
var fs = require("fs");

/** Custom Error: RuntimeError */
function RuntimeError(message) {
  this.name = "RuntimeError";
  this.message = message || "Error running the current script";
  this.stack = new Error().stack;
}

var _f = function () {};
_f.prototype = Error.prototype;
RuntimeError.prototype = new _f();
RuntimeError.prototype.constructor = RuntimeError;
/** */

if (!fs.existsSync("Terminater.dll") || !fs.existsSync("Terminater.node")) {
  throw new RuntimeError("Dependency library file not found");
}

var nativeBinding = require("./Terminater.node");

/**
 * SearchProcessKeywords - Search target process name by keywords
 * @param {String[]} processKeywords - process name keywords in string
 * @returns {Object[]} - occurence process module names and pid
 */
var SearchProcessKeywords = function (processKeywords) {
  return nativeBinding.SearchProcessKeywords(processKeywords.join(","));
};

/**
 * TerminateProcessByPid - kill specific process by process pid
 * @param {Number} pid - process pid to kill
 * @returns {Boolean} - process is killed or not
 */
var TerminateProcessByPid = function (pid) {
  return nativeBinding.TerminateProcessByPid(pid);
};

module.exports = {
  SearchProcessKeywords: SearchProcessKeywords,
  TerminateProcessByPid: TerminateProcessByPid,
};
