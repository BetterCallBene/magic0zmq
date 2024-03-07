ARG BUILDER_IMAGE_TAG=22.04-latest
FROM artifactory.ar.int:5012/agile-core/builder/ubuntu:${BUILDER_IMAGE_TAG} as builder

RUN \
  # Use a cache volume to speed up apt downloads
  --mount=type=cache,target=/var/cache/apt \
  # Disable automated cache deletion of apt, since we use a cache volume to speed up apt downloads
  rm -f /etc/apt/apt.conf.d/docker-clean; \
  apt-get -y -o=Dpkg::Use-Pty=0 update \
  && apt-get -y install --no-install-recommends \
  # Packages required for dev environment
  gcc
