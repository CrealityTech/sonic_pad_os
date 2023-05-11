$(call inherit-product-if-exists, target/allwinner/mr133-common/mr133-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := mr133_robots
PRODUCT_DEVICE := mr133-robots
PRODUCT_MODEL := mr133 MR133 robots board
