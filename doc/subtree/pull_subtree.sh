# SEIMS wiki
git fetch wiki master
git subtree pull --prefix=doc/wiki wiki master --squash
# pygeoc
git fetch pygeoc master
git subtree pull --prefix=seims/pygeoc pygeoc master --squash
# taudem
git fetch taudem master
git subtree pull --prefix=seims/src/taudem taudem master --squash
# metis
git fetch metis master
git subtree pull --prefix=seims/src/metis metis master --squash
# commonlibs
git fetch utilsclass master
git fetch mongoutilclass master
git fetch rasterclass master
git subtree pull --prefix=seims/src/commonlibs/UtilsClass utilsclass master --squash
git subtree pull --prefix=seims/src/commonlibs/MongoUtilClass mongoutilclass master --squash
git subtree pull --prefix=seims/src/commonlibs/RasterClass rasterclass master --squash
