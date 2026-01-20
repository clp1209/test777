// SPDX-License-Identifier: GPL-2.0
/*
 * ACPI_EXAMPLE.asl - Example ACPI device definition for WB CPLD
 *
 * This is a placeholder showing the expected ACPI table structure.
 * The actual HID, GPIO numbers, and IO ranges must be provided by
 * the hardware/BIOS team.
 */

/*
 * Expected ACPI Device Definition:
 *
 * Device (CPLD) {
 *     Name (_HID, "WBCPLD")  // TODO: Replace with actual HID
 *     Name (_UID, 0)
 *
 *     // GPIO resources for CS_CTL control
 *     Name (_CRS, ResourceTemplate () {
 *         // AGPIO9 for CS_CTL[0]
 *         GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly,
 *                 "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {
 *             9  // AGPIO9 pin number (TODO: verify)
 *         }
 *
 *         // AGPIO10 for CS_CTL[1]
 *         GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly,
 *                 "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {
 *             10  // AGPIO10 pin number (TODO: verify)
 *         }
 *
 *         // Optional: IO port range if not configured by BIOS
 *         // IO (Decode16, 0x0400, 0x04FF, 0x01, 0x100)
 *     })
 *
 *     // Device status
 *     Method (_STA, 0, NotSerialized) {
 *         Return (0x0F)  // Present, enabled, shown
 *     }
 * }
 *
 * Alternative GPIO resource naming (if using devm_gpiod_get_index):
 *
 * Name (_DSD, Package () {
 *     ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
 *     Package () {
 *         Package () { "cs-ctl0-gpios", Package () { ^CPLD, 0, 0, 0 } },
 *         Package () { "cs-ctl1-gpios", Package () { ^CPLD, 1, 0, 0 } },
 *     }
 * })
 */

/*
 * Driver will attempt to match using:
 *   - HID: "WBCPLD" (placeholder)
 *   - GPIO names: "cs-ctl0", "cs-ctl1"
 *
 * If resources are not found, driver probe will fail with:
 *   "Failed to get cs-ctl0 GPIO (AGPIO9)"
 *   "TODO: Check ACPI CRS resource mapping"
 */

/*
 * eSPI IO Decode Range Configuration:
 *
 * The eSPI controller must have IO decode range configured to include
 * the CPLD IO window (default 0x400~0x4FF).
 *
 * This is typically done in BIOS via:
 *   - eSPI Controller Configuration Registers
 *   - IO Decode Range Enable bits
 *
 * If not configured, driver may need to call into espi-amd driver
 * to configure it dynamically (not currently implemented).
 *
 * Reference: espi-amd.c espi_set_io_mmio_decode_config()
 */

/*
 * TODO List for ACPI Integration:
 *
 * 1. Obtain actual HID from vendor ACPI specification
 * 2. Confirm AGPIO9/10 pin numbers and polarity
 * 3. Verify IO decode range is configured in BIOS
 * 4. Test driver probe with real ACPI table
 * 5. Update wb_cpld_bus.c acpi_device_id table
 * 6. Update GPIO resource names if using different convention
 */
