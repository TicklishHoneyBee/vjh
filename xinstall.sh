#!/bin/bash
echo ""
echo "Voyager: Journey Home Installer"
echo ""

export TMPDIR=`mktemp -d /tmp/voyager.XXXXXX`

ARCHIVE=`awk '/^__ARCHIVE_BELOW__/ {print NR + 1; exit 0; }' $0`

tail -n+$ARCHIVE $0 | tar xz -C $TMPDIR

CDIR=`pwd`
cd $TMPDIR
mkdir -p $CDIR/voyager
mkdir -p $CDIR/bin
mv $TMPDIR/* $CDIR/voyager/.
cd $CDIR/voyager
rm -rf $TMPDIR
tar xf voyager.tar
rm voyager.tar

touch $CDIR/bin/voyager
echo "#!/bin/sh" > $CDIR/bin/voyager
echo "cd ${CDIR}/voyager" >> $CDIR/bin/voyager
echo "./voyager \$@" >> $CDIR/bin/voyager
chmod a+x $CDIR/bin/voyager

echo "Voyager: Journey Home Installed to ${CDIR}/voyager"

echo ""
echo "To Play, run:"
echo "${CDIR}/bin/voyager"
echo ""


exit 0

__ARCHIVE_BELOW__
