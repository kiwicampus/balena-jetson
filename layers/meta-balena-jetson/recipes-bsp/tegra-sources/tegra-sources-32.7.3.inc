L4T_BSP_NAME = "${L4T_SRCS_NAME}"
SRC_URI:tegra186 += "https://developer.nvidia.com/downloads/remack-sdksjetpack-463r32releasev73sourcest186publicsourcestbz2;downloadfilename=Jetson-public_sources-${L4T_VERSION}.tbz2"
SRC_URI:tegra194 += "https://developer.nvidia.com/downloads/remack-sdksjetpack-463r32releasev73sourcest186publicsourcestbz2;downloadfilename=Jetson-public_sources-${L4T_VERSION}.tbz2"
SRC_URI:tegra210 += "https://developer.nvidia.com/downloads/remack-sdksjetpack-463r32releasev73sourcest210publicsourcestbz2;downloadfilename=Jetson-public_sources-${L4T_VERSION}.tbz2"
SRC_URI[sha256sum] = "919b4091f0ea60dda2646feaca44bba7a7722d3343272795d9bac21797eea4eb"

inherit l4t_bsp

unpack_tar_in_tar() {
    cd ${WORKDIR}
    tar -x -j -f ${SRC_ARCHIVE} ${TEGRA_SRC_SUBARCHIVE} --to-command="tar -x -j --no-same-owner -f- ${TEGRA_SRC_SUBARCHIVE_OPTS}"
    if [ -n "${TEGRA_SRC_EXTRA_SUBARCHIVE}" ]; then
        tar -x -j -f ${SRC_ARCHIVE} ${TEGRA_SRC_EXTRA_SUBARCHIVE} --to-command="tar -x -j --no-same-owner -f- ${TEGRA_SRC_EXTRA_SUBARCHIVE_OPTS}"
    fi
}

python do_unpack() {
    src_uri = (d.getVar('SRC_URI', True) or "").split()
    rootdir = d.getVar('WORKDIR', True)
    p_dir = os.path.join(d.getVar('S', True), 'patches')
    bb.utils.remove(p_dir, True)
    try:
        fetcher = bb.fetch2.Fetch(src_uri, d)
        urldata = fetcher.ud[src_uri[0]]
        urldata.setup_localpath(d)
    except bb.fetch2.BBFetchException as e:
        raise bb.build.FuncFailed(e)

    if urldata.localpath.endswith(".tbz2"):
        d.setVar('SRC_ARCHIVE', urldata.localpath)
        bb.build.exec_func("unpack_tar_in_tar", d)
}

COMPATIBLE_MACHINE = "tegra"
PACKAGE_ARCH = "${L4T_BSP_PKGARCH}"
