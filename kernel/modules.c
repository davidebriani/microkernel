#include <kernel/lib/string.h>
#include <kernel/modules.h>

static modules_module *modules[MODULE_SLOTS];

/* Connect all devices of the same type to the driver and viceversa */
static void install_driver(modules_driver *driver) {
    modules_module *module;
    modules_device *device;
    uint32_t i;

    if (!driver->check)
        return;

    for (i = 0; i < MODULE_SLOTS; i++) {
        module = modules[i];

        if (!module)
	    continue;
	if (module->base.type != MODULE_TYPE_DEVICE)
            continue;

	device = &module->device;

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
static void attach_driver(modules_device *device) {
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
	if (module->base.type != MODULE_TYPE_DRIVER)
	    continue;

	driver = &module->driver;

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
	if (module->base.type != MODULE_TYPE_DRIVER)
            continue;

        driver = &module->driver;

        if (driver->type == type)
            return driver;
    }

    return 0;
}

/* Register a generic module */
static void register_base(modules_base *base) {
    uint32_t i;
    modules_module *module = (modules_module *) base;

    for (i = 0; i < MODULE_SLOTS; i++) {
        if (modules[i])
            continue;

        modules[i] = module;
        break;
    }
}

/* Register a bus module */
void modules_register_bus(modules_bus *bus) {
    register_base(&bus->base);

    if (bus->scan)
        bus->scan();
}

/* Register a device module */
void modules_register_device(modules_device *device) {
    register_base(&device->base);
    attach_driver(device);
}

/* Register a driver module */
void modules_register_driver(modules_driver *driver) {
    register_base(&driver->base);
    install_driver(driver);

    if (driver->start)
        driver->start();
}

/* Unregister a generic module */
static void unregister_base(modules_base *base) {
    uint32_t i;
    modules_module *module = (modules_module *) base;

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
    unregister_base(&bus->base);
}

/* Unregister a device module */
void modules_unregister_device(modules_device *device) {
    unregister_base(&device->base);
}

/* Unregister a driver module */
void modules_unregister_driver(modules_driver *driver) {
    unregister_base(&driver->base);
}

/* Setup a generic module */
static void base_init(modules_base *base, uint32_t type, const int8_t *name) {
    memclr(base, sizeof(modules_base));
    base->type = type;
    base->name = name;
}

/* Setup a bus module */
void modules_bus_init(modules_bus *bus, uint32_t type, const int8_t *name, void (*scan)(void)) {
    memclr(bus, sizeof(modules_bus));
    base_init(&bus->base, MODULE_TYPE_BUS, name);
    bus->type = type;
    bus->scan = scan;
}

/* Setup a device module */
void modules_device_init(modules_device *device, uint32_t type, const int8_t *name) {
    memclr(device, sizeof(modules_device));
    base_init(&device->base, MODULE_TYPE_DEVICE, name);
    device->type = type;
}

/* Setup a driver module */
void modules_driver_init(modules_driver *driver, uint32_t type, const int8_t *name, void (*start)(void), uint32_t (*check)(modules_device *device), void (*attach)(modules_device *device)) {
    memclr(driver, sizeof(modules_driver));
    base_init(&driver->base, MODULE_TYPE_DRIVER, name);
    driver->type = type;
    driver->start = start;
    driver->check = check;
    driver->attach = attach;
}

void modules_init(void) {
}
