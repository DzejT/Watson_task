module("luci.controller.daemon_controller", package.seeall)

function index()
    entry({"admin", "services", "daemon_model"}, cbi("daemon_model"), _("Daemon Model"),105)
end
