var path = require("path");

var Terminater = path.resolve("../").replace(/\\/gm, "\\\\");

var TerminaterLib = path.resolve("../Terminater.lib").replace(/\\/gm, "\\\\");

module.exports = {
  Terminater: Terminater,
  TerminaterLib: TerminaterLib,
};
