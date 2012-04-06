#include <kernel/lib/string.h>
#include <kernel/modules.h>

static modules_module *modules[MODULE_SLOTS];

/* Connect all devices of the same type to the driver and viceversa */
static void modules_install_driver(modules_driver *driver) {
    modules_module *module;
    modules_device *device;
    uint32_t i;

    if (!driver->check)
        return;

    for (i = 0; i < MODULE_SLOTS; i++) {
        module = modules[i];

        if (!module)
	    continue;
	if (module->type != MODULE_TYPE_DEVICE)
            continue;

        device = (modules_device *) module;

        if (device->driver)
            continue;
        if (!driver->check(device))
            continue;

        device->driver = driver;

        if (driver->attach)
            driver->attach(device);
    }
}

/* Attach a driver to device, if any suitable */
static void modules_attach_driver(modules_device *device) {
    modules_module *module;
    modules_driver *driver;
    uint32_t i;

    if (!device)
	return;
    if (device->driver)
	return;

    for (i = 0; i < MODULE_SLOTS; i++) {
	module = modules[i];

	if (!module)
	    continue;
	if (module->type != MODULE_TYPE_DRIVER)
	    continue;

	driver = (modules_device *) module;

	if (!driver->check(device))
	    continue;

	device->driver = driver;
	if (driver->attach)
	    driver->attach(device);
    }
}

/* Look for a driver of given type */
modules_driver *modules_get_driver(uint32_t type) {
    modules_module *module;
    modules_driver *driver;
    uint32_t i;

    for (i = 0; i < MODULE_SLOTS; i++) {
        module = modules[i];

        if (!module)
	    continue;
	if (module->type != MODULE_TYPE_DRIVER)
            continue;

        driver = (modules_driver *) module;

        if (driver->type == type)
            return driver;
    }

    return 0;
}

/* Register a generic module */
static void modules_register_module(modules_module *module) {
    uint32_t i;

    for (i = 0; i < MODULE_SLOTS; i++) {
        if (modules[i])
            continue;

        modules[i] = module;
        break;
    }
}

/* Register a bus module */
void modules_register_bus(modules_bus *bus) {
    modules_register_module(&bus->module);

    if (bus->scan)
        bus->scan();
}

/* Register a device module */
void modules_register_device(modules_device *device) {
    modules_register_module(&device->module);
    modules_attach_driver(device);
}

/* Register a driver module */
void modules_register_driver(modules_driver *driver) {
    modules_register_module(&driver->module);
    modules_install_driver(driver);

    if (driver->start)
        driver->start();
}

/* Unregister a generic module */
static void modules_unregister_module(modules_module *module) {
    uint32_t i;

    for (i = 0; i < MODULE_SLOTS; i++) {
        if (!modules[i])
            continue;
        if (modules[i] != module)
            continue;

        modules[i] = 0;
        break;
    }
}

/* Unregister a bus module */
void modules_unregister_bus(modules_bus *bus) {
    modules_unregister_module(&bus->module);
}

/* Unregister a device module */
void modules_unregister_device(modules_device *device) {
    modules_unregister_module(&device->module);
}

/* Unregister a driver module */
void modules_unregister_driver(modules_driver *driver) {
    modules_unregister_module(&driver->module);
}

/* Setup a generic module */
static void modules_module_init(modules_module *module, uint32_t type, const int8_t *name) {
    memclr(module, sizeof(modules_module));
    module->type = type;
    module->name = name;
}

/* Setup a bus module */
void modules_bus_init(modules_bus *bus, uint32_t type, const int8_t *name) {
    memclr(bus, sizeof(modules_bus));
    modules_module_init(&bus->module, MODULE_TYPE_BUS, name);
    bus->type = type;
}

/* Setup a device module */
void modules_device_init(modules_device *device, uint32_t type, const int8_t *name) {
    memclr(device, sizeof(modules_device));
    modules_module_init(&device->module, MODULE_TYPE_DEVICE, name);
    device->type = type;
}

/* Setup a driver module */
void modules_driver_init(modules_driver *driver, uint32_t type, const int8_t *name) {
    memclr(driver, sizeof(modules_driver));
    modules_module_init(&driver->module, MODULE_TYPE_DRIVER, name);
    driver->type = type;
}

void modules_init(void) {
}
