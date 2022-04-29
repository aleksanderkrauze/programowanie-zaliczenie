#!/usr/bin/sh

if [ -z "$1" ]; then
  ANIMATION_NAME="plots/animation.gif"
else
  ANIMATION_NAME="$1"
fi

sigint_handler() {
  echo "Received SIGINT. Removing $ANIMATION_NAME"
  if [ -f "ANIMATION_NAME" ]; then
    rm $ANIMATION_NAME
  fi
  exit 1
}

echo "Combining plots/frame_*.png into $ANIMATION_NAME"
trap "sigint_handler" INT
convert plots/frame_*.png $ANIMATION_NAME
echo "Finished"