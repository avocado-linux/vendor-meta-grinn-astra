# Upstream image_synaimg.bbclass keeps gzip subimages up to 300 MiB and splits
# larger sparse subimages into 300 MiB chunks. The 261x flash images are written
# with the 2619 SOM USB boot images, which use the 512 MB layout, and there
# U-Boot loads each subimage into a malloc buffer of about 86 MiB. Rewrite the
# 300 MiB size at parse time, so every subimage fits into this buffer.
# bb.fatal guards against silent breakage if the size is ever changed upstream.
GRINN_SYNAIMG_CHUNK_SIZE = "67108864"

python () {
    if "grinn-astra-261x-platform" not in d.getVar("MACHINEOVERRIDES").split(":"):
        return

    # The USB machines do not build the SYNAIMG image.
    if not bb.data.inherits_class("image_synaimg", d):
        return

    image_body = d.getVar("IMAGE_CMD:synaimg", False)
    if not image_body or "314572800" not in image_body:
        bb.fatal(
            "astra-media: upstream IMAGE_CMD:synaimg no longer contains the "
            "'314572800' subimage size - refactor astra-media-synaimg.inc."
        )

    d.setVar("IMAGE_CMD:synaimg", image_body.replace("314572800", "${GRINN_SYNAIMG_CHUNK_SIZE}"))
}
