# ========= Dockerfile =========
FROM ubuntu:20.04

# Disable interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Update and install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    make \
    qemu-system-misc \
    gdb-multiarch \
    git \
    wget \
    vim \
    sudo \
    ca-certificates \
    python3 \
    && rm -rf /var/lib/apt/lists/*

# Install RISC-V toolchain
RUN apt-get update && apt-get install -y \
    gcc-riscv64-unknown-elf \
    binutils-riscv64-unknown-elf \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user for convenience
RUN useradd -ms /bin/bash os && echo "os ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

USER os
WORKDIR /home/os/project

# Default command
CMD ["/bin/bash"]
