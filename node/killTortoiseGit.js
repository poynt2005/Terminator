var Terminater = require("./Terminater");

var tortoistgitKeywords = ["TortoiseGit"];

var searchResult = Terminater.SearchProcessKeywords(tortoistgitKeywords);

searchResult.forEach(function (result) {
  if (Terminater.TerminateProcessByPid(result.pid)) {
    console.log("Kill pid: " + result.pid);

    console.log("Relative module names:");
    console.log(result.modules.join("\n"));
  }
  console.log("*************");
});
