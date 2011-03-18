#ifndef KERNEL_MODULES_H
#define KERNEL_MODULES_H

#define MODULE_TYPE_BUS    1
#define MODULE_TYPE_DEVICE 2
#define MODULE_TYPE_DRIVER 3

#define MODULE_SLOTS 128

struct modules_bus;
struct modules_device;
struct modules_driver;

typedef struct modules_base {
    uint32_t type;
    const int8_t *name;
} modules_base;

typedef struct modules_bus {
    struct modules_base base;
    uint32_t type;
    void (*scan)(void);
} modules_bus;

typedef struct modules_device {
    struct modules_base base;
    uint32_t type;
    struct modules_driver *driver;
} modules_device;

typedef struct modules_driver {
    struct modules_base base;
    uint32_t type;
    void (*start)(void);
    uint32_t (*check)(struct modules_device *device);
    void (*attach)(struct modules_device *device);
} modules_driver;

typedef union modules_module {
    modules_base base;
    modules_bus bus;
    modules_device device;
    modules_driver driver;
} modules_module;

modules_driver *modules_get_driver(uint32_t type);
void modules_register_bus(modules_bus *bus);
void modules_register_device(modules_device *device);
void modules_register_driver(modules_driver *driver);
void modules_unregister_bus(modules_bus *bus);
void modules_unregister_device(modules_device *device);
void modules_unregister_driver(modules_driver *driver);
void modules_bus_init(modules_bus *bus, uint32_t type, const int8_t *name, void (*scan)(void));
void modules_device_init(modules_device *device, uint32_t type, const int8_t *name);
void modules_driver_init(modules_driver *driver, uint32_t type, const int8_t *name, void (*start)(void), uint32_t (*check)(modules_device *device), void (*attach)(modules_device *device));
void modules_init(void);

#endif
