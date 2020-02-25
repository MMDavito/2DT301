console.log("HURAY FOR ME!");
yesnoCheckProgram

function yesnoCheckProgram() {
    if (document.getElementById("noCheckProgram").checked) {
        $(".static_div").show();
        $(".dynamic_div").hide();
    } else {
        $(".static_div").hide();
        $(".dynamic_div").show();
    }
}
function yesnoCheck() {//check eachHastStartTime
    if (document.getElementById("noCheck").checked) {
        $(".div_start_time").hide();
        $("#num_repeats").show();

    } else {
        $(".div_start_time").show();
        $("#num_repeats").hide();
    }
}
function printBajs() {
    console.log("BAJS");;
}
function clearSkit() {
    console.log("Will now clear");
    var data = sessionStorage.getItem("relays");
    //console.log("SHITTY DATA:\n"+data);
    data = JSON.parse(data);
    //console.log("SHITTiest DATA:\n"+JSON.stringify(data["data"]));
    var relays = data["data"];
    relays["relays"].forEach(relay => {
        document.getElementById("isOff" + relay["id"]).checked = true;
    })
}
function sendSkit() {
    if (document.getElementById("noCheckProgram").checked) {
        sendStatic();
        return;
    }//ELSE:   
    sendDynamic();
    return;
}
function sendDynamic() {
    maxTimePerDay = 1000 * 60 * 60 * 24;//Will need to test this by changing serverresponse.
    console.log("######################################################################################");
    var hasStartTime = true;//TODO: set to true
    var eachHasStartTime = false;
    var defaultSeconds = 4;//TODO MAKE 0!
    console.log("Will now send using dynamic/programerfull info.");
    var startTime = document.getElementById("start_time_0");
    var start = startTime.value;
    console.log("STARTIME length: " + startTime.value.length);
    console.log("STARTIME value: " + startTime.value);
    if (start.length == 8) {
        console.log("STARTIME hours: " + parseInt(start.substring(0, start.indexOf(":"))));
        console.log("STARTIME minutes: " + parseInt(start.substring(start.indexOf(":") + 1, start.lastIndexOf(":"))));//remember to change to utc!(if == 0, make it 23!)
        console.log("STARTIME minutes: " + (start.substring(start.indexOf(":") + 1, start.lastIndexOf(":"))));//remember to change to utc!(if == 0, make it 23!)
        console.log("STARTIME seconds: " + parseInt(start.substring(start.lastIndexOf(":") + 1, start.length)));
    } else if (start.length == 5) {
        console.log("STARTIME hours: " + parseInt(start.substring(0, start.indexOf(":"))));
        console.log("STARTIME minutes: " + (start.substring(start.indexOf(":") + 1, start.length)));
        console.log("STARTIME seconds: " + 0);
    }


    if (startTime.value.length == 0 || !hasStartTime) {//TODO ON clear if dynamic, reset all times to null...
        console.log("No starttime will be allowed.");
        hasStartTime = false;
        eachHasStartTime = false
    }
    else if (document.getElementById("yesCheck").checked) {
        //It allows for setting dates.
        //TODO: change in the forEach so I set all nonExistant to the last exisisting, so they run in sequence...
        console.log("NOT SUPPORTED #yesCheck will need to be implemented");
        eachHasStartTime = true;//if one lacks starttime it will start at previous+previousDuration..
    }

    var data = sessionStorage.getItem("relays");
    //console.log("SHITTY DATA:\n"+data);
    data = JSON.parse(data);
    //console.log("SHITTiest DATA:\n"+JSON.stringify(data["data"]));

    var content = data["data"];
    var relays = content["relays"];
    //var relays = relays;
    content = { "is_dynamic": true };
    //content["is_dynamic"] = true;
    //var relayInfos = [];//Info about/for the relays.
    var i = 0;
    relays.forEach(relay => {
        //var id = relay["id"];
        //relay = {"id":id};

        console.log("HEJJJJJ: " + JSON.stringify(relay));
        var milliseconds = 0;
        //console.log("seconds_"+relay["id"]);
        var seconds = document.getElementById("seconds_" + relay["id"]).value;
        console.log("SECONDS length: " + seconds.length + ", Seconds value: " + seconds);
        if (seconds.length == 0) {
            seconds = defaultSeconds;
        }
        milliseconds += seconds * 1000;

        var minutes = document.getElementById("minutes_" + relay["id"]).value;
        console.log("MINUTES length: " + minutes.length + ", Minutes value: " + minutes);
        if (minutes.length == 0) {
            minutes = 0;
        }
        milliseconds += minutes * 60 * 1000;

        var hours = document.getElementById("hours_" + relay["id"]).value;
        console.log("Hours length: " + hours.length + ", hours value: " + hours);

        if (hours.length == 0) {
            hours = 0;
        }
        milliseconds += hours * 60 * 60 * 1000;
        //if (milliseconds == 0) milliseconds = miniDuration;
        relay["duration"] = milliseconds;
        milliseconds = 0;
        repeats = document.getElementById("num_repeats").value;
        if (repeats.length == 0) repeats = -1;//standardIfHasStartTime
        else repeats = parseInt(repeats);
        //If does not have startTime it will repat the program for ever.
        // only interesting ifEach hasStarttime == false
        content["num_repeats"] = repeats;

        if (hasStartTime && i == 0) {
            var startTime = document.getElementById("start_time_" + relay["id"]);
            var start = startTime.value;
            console.log("STARTIME length: " + startTime.value.length);
            console.log("STARTIME value: " + startTime.value);
            var hours = -1;
            var minutes = -1;
            var seconds = -1;
            if (start.length == 0) {
                alert("Should never happen, screenshot and send to David!");//alredy tested.
                hasStartTime = false;
                eachHasStartTime = false;
            }
            else if (start.length == 8) {
                hours = parseInt(start.substring(0, start.indexOf(":")));
                /*if (hours == 0) hours = 23;
                else hours = hours - 1;*/
                minutes = parseInt(start.substring(start.indexOf(":") + 1, start.lastIndexOf(":")));//remember to change to utc!(if == 0, make it 23!)
                seconds = parseInt(start.substring(start.lastIndexOf(":") + 1, start.length));

                console.log("STARTIME hours: " + hours);
                console.log("STARTIME minutes: " + minutes);
                console.log("STARTIME seconds: " + seconds)
            } else if (start.length == 5) {
                hours = parseInt(start.substring(0, start.indexOf(":")));
                /*if (hours == 0) hours = 23;
                else hours = hours - 1;*/
                minutes = parseInt(start.substring(start.indexOf(":") + 1, start.length));
                seconds = 0;
                console.log("STARTIME hours: " + hours);
                console.log("STARTIME minutes: " + minutes);
                console.log("STARTIME seconds: " + 0);
            } else {
                alert("IS NOT VALID TIME FOR RELAY: " + relay["id"]);
                hasStartTime = false;
                eachHasStartTime = false;
            }
            if (hasStartTime) {
                milliseconds = 0;
                milliseconds += seconds * 1000;
                milliseconds += minutes * 1000 * 60;
                milliseconds += hours * 1000 * 60 * 60;
                relay["start_time"] = milliseconds;
                relay["shitty_time"] = start;
            }
        } else if (eachHasStartTime && i > 0) {
            var startTime = document.getElementById("start_time_" + relay["id"]);
            var start = startTime.value;
            console.log("STARTIME length: " + startTime.value.length);
            console.log("STARTIME value: " + startTime.value);
            var hours = -1;
            var minutes = -1;
            var seconds = -1;
            milliseconds = 0;
            if (start.length == 0) {//Choose previous relay time+duration
                milliseconds = relays[i - 1]["start_time"] + relays[i - 1]["duration"];
                if (milliseconds >= maxTimePerDay) {
                    console.log("maX TIME: " + maxTimePerDay);
                    console.log("millis: " + milliseconds);

                    temp = milliseconds - maxTimePerDay;
                    milliseconds = temp;
                }
            }
            else if (start.length == 8) {
                milliseconds = 0;
                hours = parseInt(start.substring(0, start.indexOf(":")));
                /*if (hours == 0) hours = 23;
                else hours = hours - 1;
                */
                minutes = parseInt(start.substring(start.indexOf(":") + 1, start.lastIndexOf(":")));//remember to change to utc!(if == 0, make it 23!)
                seconds = parseInt(start.substring(start.lastIndexOf(":") + 1, start.length));

                console.log("STARTIME hours: " + hours);
                console.log("STARTIME minutes: " + minutes);
                console.log("STARTIME seconds: " + seconds);
            } else if (start.length == 5) {
                milliseconds = 0;
                hours = parseInt(start.substring(0, start.indexOf(":")));
                /*if (hours == 0) hours = 23;
                else hours = hours - 1;*/
                minutes = parseInt(start.substring(start.indexOf(":") + 1, start.length));
                seconds = 0;
                milliseconds += seconds * 1000;
                milliseconds += minutes * 1000 * 60;
                milliseconds += hours * 1000 * 60 * 60;
                console.log("STARTIME hours: " + hours);
                console.log("STARTIME minutes: " + minutes);
                console.log("STARTIME seconds: " + 0);
            } else {
                alert("IS NOT VALID TIME FOR RELAY: " + relay["id"]);
                hasStartTime = false;
                eachHasStartTime = false;
            }
            if (eachHasStartTime) {
                relay["start_time"] = milliseconds;
                relay["shitty_time"] = start;
            }
        } else if (i > 0 && hasStartTime && !eachHasStartTime) {
            var startTime = document.getElementById("start_time_" + relay["id"]);
            var start = startTime.value;
            console.log("STARTIME length: " + startTime.value.length);
            console.log("STARTIME value: " + startTime.value);
            var hours = -1;
            var minutes = -1;
            var seconds = -1;
            milliseconds = 0;
            if (start.length == 0) {//Choose previous relay time+duration
                milliseconds = relays[i - 1]["start_time"] + relays[i - 1]["duration"];
                if (milliseconds >= maxTimePerDay) {
                    console.log("maX TIME: " + maxTimePerDay);
                    console.log("millis: " + milliseconds);
                    temp = milliseconds - maxTimePerDay;
                    milliseconds = temp;
                }
            }
            relay["start_time"] = milliseconds;
            relay["shitty_time"] = start;
        }
        console.log("RELAY AFTER: " + JSON.stringify(relay));
        relays[i] = relay;
        i++;
    });
    content["has_start_time"] = hasStartTime;
    content["each_has_start_time"] = eachHasStartTime;
    content["relays"] = relays;
    data["data"] = content;


    console.log("RELAYS AFTER: " + JSON.stringify(relays));
    console.log("DATA AFTER: " + JSON.stringify(data));
    console.log("Content AFTER: " + JSON.stringify(content));
    sessionStorage.setItem("relays", JSON.stringify(data));
    console.log("Will now post!");
    $.ajax({
        url: "/arduino_relay",
        type: 'POST',
        dataType: 'json',
        data: JSON.stringify(data["data"]),
        //headers:{"credentials":"BAJS"},
        //credentials:"BAJS",
        beforeSend: function (request) {
            request.setRequestHeader("x-api-key", 1337);
            request.setRequestHeader("Credentials", "BAJS");
        },

        contentType: 'application/json; charset=utf-8',
        success: function (result) {
            console.log("RESULTATFAN EFTER: \n" + result)
        },
        error: function (request, status, error) {
            console.log("request: " + request);
            console.log("status: " + status);
            console.log("ERROR: " + error);
        }
    });

}
function sendStatic() {
    console.log("Will now SEND using the static/ programerless info.");
    var data = sessionStorage.getItem("relays");
    //console.log("SHITTY DATA:\n"+data);
    data = JSON.parse(data);
    //console.log("SHITTiest DATA:\n"+JSON.stringify(data["data"]));
    var relays = data["data"]["relays"];
    data["data"] = { "relays": relays };
    //relays = data["data"];
    var i = 0;
    relays.forEach(relay => {
        var id = relay["id"];
        relay = { "id": id };
        var isOn = false;
        //console.log("Does this work?: "+"isOn"+relay["id"]);
        //console.log("IS TEMPPPP?: "+temp);
        if (document.getElementById("isOn" + relay["id"]).checked) {
            isOn = true;
        }
        console.log("ID: " + relay["id"] + ", is on?: " + isOn);
        relay["relay_is_on"] = isOn;
        relays[i] = relay;
        i++;
        //relay["relay_is_on"] = document.getElementById("isON"+relay["id"]).checked;
    })
    console.log("Relays after:\n" + JSON.stringify(relays));
    console.log("DATA after:\n" + JSON.stringify(data));

    sessionStorage.setItem("relays", JSON.stringify(data));
    $.ajax({
        url: "/arduino_relay",
        type: 'POST',
        dataType: 'json',
        data: JSON.stringify(data["data"]),
        //headers:{"credentials":"BAJS"},
        //credentials:"BAJS",
        beforeSend: function (request) {
            request.setRequestHeader("x-api-key", 1337);
            request.setRequestHeader("Credentials", "BAJS");
        },

        contentType: 'application/json; charset=utf-8',
        success: function (result) {
            console.log("RESULTATFAN EFTER: \n" + result)
        },
        error: function (request, status, error) {
            console.log("request: " + request);
            console.log("status: " + status);
            console.log("ERROR: " + error);
        }
    });
}

$.ajax({
    url: "/arduino_relay",
    type: 'GET',
    dataType: 'json',
    //headers:{"credentials":"BAJS"},
    //credentials:"BAJS",
    beforeSend: function (request) {
        request.setRequestHeader("x-api-key", 1337);
        request.setRequestHeader("Credentials", "BAJS");
    },

    contentType: 'application/json; charset=utf-8',
    success: function (result) {
        sessionStorage.setItem("relays", JSON.stringify(result));
        console.log("RESULTATFAN: " + JSON.stringify(result));
        console.log("Är detta data?: " + result["data"]);
        var data = result["data"]["relays"];
        //sessionStorage.setItem("relays",result["data"]);

        var li = document.getElementById("listfan");
        var FormBajs = document.getElementById("FormBajs");
        var i = 0;
        data.forEach(element => {
            var textNode = document.createTextNode(JSON.stringify(element));
            var node = document.createElement("LI");
            node.appendChild(textNode);
            li.appendChild(node);

            //TODO: WIll try to make child of label id "ON" or "OFF".
            var formStatic = document.createElement("div");
            formStatic.classList.add("static_div");
            var formDynamic = document.createElement("div");
            formDynamic.classList.add("dynamic_div");
            var formDiv = document.createElement("div");
            formDiv.id = "form_relay_" + element["id"];
            formDiv.classList.add("panel");
            formDiv.classList.add("panel-default");
            formDiv.classList.add("form-check");
            //formDiv.innerHTML("<h1>Relay: "+data["id"]+"</h1>");
            console.log("ID?: " + element["id"]);
            var innerText = document.createTextNode("Relay " + element["id"] + ": ");
            formDiv.appendChild(innerText);
            var lineBreak = document.createElement("br");
            formDiv.appendChild(lineBreak);

            var labelON = document.createElement("label");
            var radioON = document.createElement("input");
            labelON.classList.add("form-check-label");
            radioON.classList.add("form-check-input");

            radioON.type = "radio";
            radioON.name = "yes_no_" + element["id"];
            radioON.id = "isOn" + element["id"];
            radioON.value = "isOn";
            if (element["relay_is_on"]) console.log("Helvetes skit");
            //radioON.checked = element["relay_is_on"];
            //radioON.checked =true;

            labelON.appendChild(radioON);
            labelON.innerHTML += "ON";
            formStatic.appendChild(labelON);

            var labelOFF = document.createElement("label");
            var radioOFF = document.createElement("input");
            labelOFF.classList.add("form-check-label");
            radioOFF.classList.add("form-check-input");
            radioOFF.name = "yes_no_" + element["id"];
            radioOFF.type = "radio";
            radioOFF.id = "isOff" + element["id"];
            radioOFF.value = "isOff";
            labelOFF.appendChild(radioOFF);
            labelOFF.innerHTML += "OFF";
            formStatic.appendChild(labelOFF);
            var lineBreak = document.createElement("br");
            formStatic.appendChild(lineBreak);

            var textNode = document.createTextNode("Duration: ");
            var lineBreak = document.createElement("br");
            formDynamic.appendChild(textNode);
            formDynamic.appendChild(lineBreak);
            //Hours
            var label = document.createElement("label");
            var input = document.createElement("input");
            label.innerHTML += "Hours: ";
            label.classList.add("form-check-label");
            input.classList.add("form-check-input");
            input.id = "hours_" + element["id"];
            input.type = "number";
            input.name = "hours" + element["id"];
            input.min = "0";
            input.max = "10";
            label.appendChild(input);
            formDynamic.appendChild(label);
            //Minutes
            label = document.createElement("label");
            input = document.createElement("input");
            label.innerHTML += " Minutes: ";
            label.classList.add("form-check-label");
            input.classList.add("form-check-input");
            input.id = "minutes_" + element["id"];
            input.type = "number";
            input.name = "minutes" + element["id"];
            input.min = "0";
            input.max = "60";
            label.appendChild(input);
            formDynamic.appendChild(label);
            //Seconds
            label = document.createElement("label");
            input = document.createElement("input");
            label.innerHTML += " Seconds: ";
            label.classList.add("form-check-label");
            input.classList.add("form-check-input");
            input.id = "seconds_" + element["id"];
            input.type = "number";
            input.name = "seconds" + element["id"];
            input.min = "0";
            input.max = "60";
            label.appendChild(input);
            formDynamic.appendChild(label);
            var lineBreak = document.createElement("br");

            formDynamic.appendChild(lineBreak);


            label = document.createElement("label");
            input = document.createElement("input");
            label.classList.add("form-check-label");
            input.classList.add("form-check-input");
            label.innerHTML += "Start time: ";
            input.id = "start_time_" + element["id"];//null if start imidetly (willl)
            input.type = "time";
            input.step = "1";
            label.append(input);
            if (i != 0) {
                var divStartTime = document.createElement("div");
                divStartTime.classList.add("div_start_time");
                divStartTime.appendChild(label);
                formDynamic.appendChild(divStartTime);
            }
            else {
                formDynamic.appendChild(label);
            }

            if (i == 0) {
                var lineBreak = document.createElement("br");
                formDynamic.appendChild(lineBreak);

                label = document.createElement("label");
                input = document.createElement("input");
                label.classList.add("form-check-label");
                input.classList.add("form-check-input");
                label.innerHTML += " Number of repeats (-1 if infinite): ";

                input.id = "num_repeats";
                input.type = "number";
                input.name = "num_repeats" + element["id"];
                input.min = "-1";
                input.max = "10";
                label.appendChild(input);

                formDynamic.appendChild(label);

            }
            formDiv.appendChild(formStatic);
            formDiv.appendChild(formDynamic)
            FormBajs.appendChild(formDiv);
            document.getElementById("isOn" + element["id"]).checked = element["relay_is_on"];
            document.getElementById("isOff" + element["id"]).checked = !element["relay_is_on"];
            //formDiv.appendChild(lineBreak);
            i++;
        });

        if (document.getElementById("noCheck").checked) {
            yesnoCheck();
        }

        if (document.getElementById("noCheckProgram").checked) {
            $(".static_div").show();
            $(".dynamic_div").hide();
        } else {
            $(".static_div").hide();
            $(".dynamic_div").show();
        }
    },
    error: function (request, status, error) {
        console.log("request: " + request);
        console.log("status: " + status);
        console.log("ERROR: " + error);
    }

});
