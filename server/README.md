# server

| **Maintainer**        	 | @b.koenig       	 |
|-------------------------|-------------------------------------|
| **Core Contributors** 	 | TODO                                   |


zmq server

# Version Number management

To simplify changing the version number in different files, server includes the [bump2version](https://github.com/c4urself/bump2version) tool.

To update the Version, use: 
1. `pip install bump2version` to install dependencies
2. Run `bump2version {part}` where `{part}` is major / minor / patch - part
  of the version that should be increased. When working with release candidates, you can also specify "prerelease" as part.

> **Note:** `~/.local/bin` should be in your PATH to use the `bump2version` command. 

## Getting started
### For developers

If you are planning to contribute to the code base, this is how you can start using Agile Core
from source.

There are various options to start Agile Core:

#### Option 1: Development Setup

##### Using DevPods

Using this option, a development environment will be started inside a Docker Container using DevPods.
You can choose different IDEs to connect to this setup. See https://devpod.sh/

1. DevPod is pre-installed on our managed Ubuntu 22.04 PCs. Create an IT ticket if this is not the case, or [install 
   DevPod UI manually](https://devpod.sh/docs/getting-started/install)
2. Open this repository, by either clicking one of the following links or checking out the repository and opening it manually:
    - [:rocket: Open in DevPod UI (HTTP Clone)](https://devpod.sh/open#http://git.ar.int/sd/peac/mv/server.git)
    - [:helicopter: Open in DevPod UI (SSH Clone)](https://devpod.sh/open#git@git.ar.int:sd/peac/mv/server.git)
    - Clone the repo manually, and choose the cloned folder in the DevPod UI
3. Configure the following settings:
    - Choose `Docker` as the provider
    - Choose your desired IDE
    - Set a Workspace Name, such as "ar_example"
    - Do **NOT** set Prebuild Repository or DevContainer path
      Press `Create Workspace`

**NOTE:**
You can also use the devpod command line tool to start the devpod container.
And then use `ssh server.devpod` to connect to the running container.

##### Using the Visual Studio Dev Container

As an alternative, you can also run the development setup using the
[Visual Studio Dev Containers Extension](https://code.visualstudio.com/docs/devcontainers/containers).
It will use the files in `.devcontainer` to allow you to work directly in the container.
To do so:

* Open this repository in Visual Studio code
* Make sure you don't have other AC containers running.
  If you have, stop them before the next command.
* Open the command palette (by default <kbd>CTRL</kbd>+<kbd>SHIFT</kbd>+<kbd>P</kbd>)
  and execute *Dev Containers: Open Folder in Container*.
  As folder, choose the `server` directory.
  Alternatively, you can click on the blue button in the bottom left corner
  and select *Reopen in container*.

##### Using Docker Compose

If you prefer to use Docker Compose directly, you can use the following scripts:

```bash
.devcontainer/start_dev.sh
.devcontainer/stop_dev.sh
```

##### Reset permissions on files

To reset permissions, please use the following commands:
```bash
cd <directory where files with wrong permissions are located>
docker run -v $PWD:/host ubuntu:22.04 /bin/bash -c "chown -R $UID:$UID /host"
```