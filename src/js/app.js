Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://thomasfiveash.com/config/';

  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));

  console.log('Configuration page returned: ' + JSON.stringify(configData));

  if (configData) {
    Pebble.sendAppMessage({
      unitSystem: configData.unitSystem,
      barWeight: configData.barWeight,
      squatOne: configData.squatOne*1000,
      squatTwo: configData.squatTwo*1000,
      squatThree: configData.squatThree*1000,
      squatFour: configData.squatFour*1000,
      benchOne: configData.benchOne*1000,
      benchTwo: configData.benchTwo*1000,
      benchThree: configData.benchThree*1000,
      benchFour: configData.benchFour*1000,
      deadOne: configData.deadOne*1000,
      deadTwo: configData.deadTwo*1000,
      deadThree: configData.deadThree*1000,
      pressOne: configData.pressOne*1000,
      pressTwo: configData.pressTwo*1000,
      pressThree: configData.pressThree*1000,
      pressFour: configData.pressFour*1000,
      rowOne: configData.rowOne*1000,
      rowTwo: configData.rowTwo*1000,
      rowThree: configData.rowThree*1000,
      rowFour: configData.rowFour*1000,
      cleanOne: configData.cleanOne*1000,
      cleanTwo: configData.cleanTwo*1000,
      cleanThree: configData.cleanThree*1000,
      cleanFour: configData.cleanFour*1000
    }, function() {
      console.log('Send successful!');
    }, function() {
      console.log('Send failed!');
    });
  }
});