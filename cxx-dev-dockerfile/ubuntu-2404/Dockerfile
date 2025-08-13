FROM ubuntu:24.04

LABEL maintainer="kingsamchen@gmail.com"
LABEL release-date="2025-08-13"

ARG USER=dev
ARG UID=1000
ARG GID=1000

COPY ./ubuntu.sources /etc/apt/sources.list.d/ubuntu.sources

RUN apt-get update && apt-get install -y \
        build-essential \
        cmake clang clangd curl \
        git gdb \
        netcat-openbsd net-tools ninja-build \
        python3 python3-pip \
        sudo \
        vim \
    && apt-get clean

RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang 40 && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 40

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

# Remove existing "regular" users (uid >= 1000, excluding nobody), but KEEP their groups.
RUN set -eux; \
  mapfile -t REGULAR_USERS < <(awk -F: '$3>=1000 && $1!="nobody"{print $1}' /etc/passwd || true); \
  for U in "${REGULAR_USERS[@]}"; do \
    # delete user and its home; ignore errors if already missing
    userdel -r "$U" || true; \
  done

# Ensure a group with GID=${GID} exists; reuse if present (whatever its name is).
RUN set -eux; \
  if getent group "${GID}" >/dev/null; then \
    TARGET_GROUP_NAME="$(getent group "${GID}" | cut -d: -f1)"; \
  else \
    groupadd -g "${GID}" "${USER}"; \
    TARGET_GROUP_NAME="${USER}"; \
  fi; \
  echo "${TARGET_GROUP_NAME}" > /etc/.target_group_name

# Create our user with UID=${UID} and PRIMARY GROUP = GID=${GID} (the group from step 2).
# If some user already owns UID=${UID}, remove that user first (keep its group).
RUN set -eux; \
  if getent passwd "${UID}" >/dev/null; then \
    OLDUSER="$(getent passwd "${UID}" | cut -d: -f1)"; \
    userdel -r "${OLDUSER}" || true; \
  fi; \
  TARGET_GROUP_NAME="$(cat /etc/.target_group_name)"; \
  useradd -m -u "${UID}" -g "${TARGET_GROUP_NAME}" -s /bin/bash "${USER}"

# sudo without password for convenience
RUN set -eux; \
  apt-get update && apt-get install -y sudo && rm -rf /var/lib/apt/lists/*; \
  echo "${USER} ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/${USER}; \
  chmod 0440 /etc/sudoers.d/${USER}

USER ${USER}
WORKDIR /home/${USER}

COPY --chown=${USER}:${USER} ./gitconfig /home/${USER}/.gitconfig
COPY --chown=${USER}:${USER} ./vimrc /home/${USER}/.vimrc
