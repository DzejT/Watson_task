
map = Map("daemon", "Daemon model")

s1 = map:section(NamedSection, "enable_sector", "enabling", "Enable")
s2 = map:section(NamedSection, "watson_sector", "watson", "Watson configs")

flag = s1:option(Flag, "enable", "Enable", "Enable program")

text1 = s2:option( Value, "deviceId", "Device ID", "IBM Watson device id for logging")
text1.placeholder = "1234"

text2 = s2:option( Value, "orgId", "Organization ID", "IBM Watson organization id for logging")
text2.placeholder = "gut697"

text3 = s2:option( Value, "token", "Authentication token", "IBM Watson authentication token for logging")
text3.maxlength = 30
text3.datatype = "string"
text3.placeholder = "P0Fo-wAItaV00_fzRN"


text4 = s2:option( Value, "typeId", "Device Type", "IBM Watson device type for logging")
text4.placeholder = "home_pc"

return map


