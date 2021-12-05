(function() {
  loadOptions();
  submitHandler();
})();

function submitHandler() {
  var $submitButton = $('#submitButton');

  $submitButton.on('click', function() {
    console.log('Submit');

    var return_to = getQueryParam('return_to', 'pebblejs://close#');
    document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
  });
}

function loadOptions() {
  var $unit = document.getElementsByName("unit-tab");
  var $barbellWeight = $('#barbell-weight');
  var $squat1 = $('#input-squat1');
  var $squat2 = $('#input-squat2');
  var $squat3 = $('#input-squat3');
  var $squat4 = $('#input-squat4');
  var $bench1 = $('#input-bench1');
  var $bench2 = $('#input-bench2');
  var $bench3 = $('#input-bench3');
  var $bench4 = $('#input-bench4');
  var $dead1 = $('#input-dead1');
  var $dead2 = $('#input-dead2');
  var $dead3 = $('#input-dead3');
  var $press1 = $('#input-press1');
  var $press2 = $('#input-press2');
  var $press3 = $('#input-press3');
  var $press4 = $('#input-press4');
  var $row1 = $('#input-row1');
  var $row2 = $('#input-row2');
  var $row3 = $('#input-row3');
  var $row4 = $('#input-row4');
  var $clean1 = $('#input-clean1');
  var $clean2 = $('#input-clean2');
  var $clean3 = $('#input-clean3');
  var $clean4 = $('#input-clean4');



  if (localStorage.unitSystem) {
    $unit[localStorage.unitSystem].className = "tab-button active";
    $unit[1-localStorage.unitSystem].className = "tab-button";
    $barbellWeight[0].value = localStorage.barWeight;
    $squat1[0].value = localStorage.squatOne;
    $squat2[0].value = localStorage.squatTwo;
    $squat3[0].value = localStorage.squatThree;
    $squat4[0].value = localStorage.squatFour;
    $bench1[0].value = localStorage.benchOne;
    $bench2[0].value = localStorage.benchTwo;
    $bench3[0].value = localStorage.benchThree;
    $bench4[0].value = localStorage.benchFour;
    $dead1[0].value = localStorage.deadOne;
    $dead2[0].value = localStorage.deadTwo;
    $dead3[0].value = localStorage.deadThree;
    $press1[0].value = localStorage.pressOne;
    $press2[0].value = localStorage.pressTwo;
    $press3[0].value = localStorage.pressThree;
    $press4[0].value = localStorage.pressFour;
    $row1[0].value = localStorage.rowOne;
    $row2[0].value = localStorage.rowTwo;
    $row3[0].value = localStorage.rowThree;
    $row4[0].value = localStorage.rowFour;
    $clean1[0].value = localStorage.cleanOne;
    $clean2[0].value = localStorage.cleanTwo;
    $clean3[0].value = localStorage.cleanThree;
    $clean4[0].value = localStorage.cleanFour;
  }
}

function getAndStoreConfigData() {
  var $unit = document.getElementsByName("unit-tab")
  var $barbellWeight = $('#barbell-weight');
  var $squat1 = $('#input-squat1');
  var $squat2 = $('#input-squat2');
  var $squat3 = $('#input-squat3');
  var $squat4 = $('#input-squat4');
  var $bench1 = $('#input-bench1');
  var $bench2 = $('#input-bench2');
  var $bench3 = $('#input-bench3');
  var $bench4 = $('#input-bench4');
  var $dead1 = $('#input-dead1');
  var $dead2 = $('#input-dead2');
  var $dead3 = $('#input-dead3');
  var $press1 = $('#input-press1');
  var $press2 = $('#input-press2');
  var $press3 = $('#input-press3');
  var $press4 = $('#input-press4');
  var $row1 = $('#input-row1');
  var $row2 = $('#input-row2');
  var $row3 = $('#input-row3');
  var $row4 = $('#input-row4');
  var $clean1 = $('#input-clean1');
  var $clean2 = $('#input-clean2');
  var $clean3 = $('#input-clean3');
  var $clean4 = $('#input-clean4');

  var options = {
    unitSystem: parseFloat(getUnitSystem()),
    barWeight: parseFloat(getBarbellWeight(getUnitSystem(), $barbellWeight.val())),
    squatOne: parseFloat($squat1.val()),
    squatTwo: parseFloat($squat2.val()),
    squatThree: parseFloat($squat3.val()),
    squatFour: parseFloat($squat4.val()),
    benchOne: parseFloat($bench1.val()),
    benchTwo: parseFloat($bench2.val()),
    benchThree: parseFloat($bench3.val()),
    benchFour: parseFloat($bench4.val()),
    deadOne: parseFloat($dead1.val()),
    deadTwo: parseFloat($dead2.val()),
    deadThree: parseFloat($dead3.val()),
    pressOne: parseFloat($press1.val()),
    pressTwo: parseFloat($press2.val()),
    pressThree: parseFloat($press3.val()),
    pressFour: parseFloat($press4.val()),
    rowOne: parseFloat($row1.val()),
    rowTwo: parseFloat($row2.val()),
    rowThree: parseFloat($row3.val()),
    rowFour: parseFloat($row4.val()),
    cleanOne: parseFloat($clean1.val()),
    cleanTwo: parseFloat($clean2.val()),
    cleanThree: parseFloat($clean3.val()),
    cleanFour: parseFloat($clean4.val())
  };

  localStorage.unitSystem = options.unitSystem;
  localStorage.barWeight = options.barWeight;
  localStorage.squatOne = options.squatOne;
  localStorage.squatTwo = options.squatTwo;
  localStorage.squatThree = options.squatThree;
  localStorage.squatFour = options.squatFour;
  localStorage.benchOne = options.benchOne;
  localStorage.benchTwo = options.benchTwo;
  localStorage.benchThree = options.benchThree;
  localStorage.benchFour = options.benchFour;
  localStorage.deadOne = options.deadOne;
  localStorage.deadTwo = options.deadTwo;
  localStorage.deadThree = options.deadThree;
  localStorage.pressOne = options.pressOne;
  localStorage.pressTwo = options.pressTwo;
  localStorage.pressThree = options.pressThree;
  localStorage.pressFour = options.pressFour;
  localStorage.rowOne = options.rowOne;
  localStorage.rowTwo = options.rowTwo;
  localStorage.rowThree = options.rowThree;
  localStorage.rowFour = options.rowFour;
  localStorage.cleanOne = options.cleanOne;
  localStorage.cleanTwo = options.cleanTwo;
  localStorage.cleanThree = options.cleanThree;
  localStorage.cleanFour = options.cleanFour;

  console.log('Got options: ' + JSON.stringify(options));
  return options;
}

function getUnitSystem() {
  var tabs = document.getElementsByName("unit-tab");
  for (var t = 0; t < tabs.length; t++) {
    var tbclasses = tabs[t].className.split(" ");
    for ( var i = 0; i < tbclasses.length; i++) {
      if (tbclasses[i] === "active") { 
        return t;
      }
    }
  }
}

function getBarbellWeight(unit, weight) {
  if (weight == "") {
    if (unit == 0 ) { return 45; } else { return 20; }
  } else { return weight; }
}

function getQueryParam(variable, defaultValue) {
  var query = location.search.substring(1);
  var vars = query.split('&');
  for (var i = 0; i < vars.length; i++) {
    var pair = vars[i].split('=');
    if (pair[0] === variable) {
      return decodeURIComponent(pair[1]);
    }
  }
  return defaultValue || false;
}
