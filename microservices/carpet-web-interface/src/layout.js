/*
Copyright 2018 @ Pontus Laestadius

Register keys to be displayed on the UI.

The input is formatted as such:
KeyCode or 'c'haracter, field, messageid, [down value, up value], label.

*/


registerkey(); // This is not displayed
registerkey(38, "percent", 1541, ["range", 0.0], "forward");
registerkey(); // This is not displayed
registerkey(37, "steeringAngle", 1545, [10, 0], "left");
registerkey(40, "percent", 1541, [-0.15, 0.0], "reverse");
registerkey(39, "steeringAngle", 1545, [-10, 0], "right");

registerkey();registerkey();registerkey(); // Create some whitespace

registerkey('a', "vehicleIp", 1001, ["\"192.168.43.135\", \"groupId\":\"6\"", null], "[a]p");
registerkey('f', "", 1002, [null, null], "[f]req");
//registerkey('r', "", 1003, [null, null], "f[r]es");
registerkey('s', "", 1004, [null, null], "[s]tpf");
//registerkey('o', "", 3001, [null, null], "f[o]ls");
