# -*- mode: ruby -*-
# vi: set ft=ruby :

# Vagrantfile API/syntax version. Don't touch unless you know what you're doing!
VAGRANTFILE_API_VERSION = "2"
$script_trusty32 = <<SCRIPT
apt-get update
apt-get install -y libwxgtk3.0-dev libboost-dev cmake build-essential libc6-dev build-essential cpp dkms g++ gcc gcc-multilib virtualbox-guest-dkms libf2c2 libf2c2-dev f2c
SCRIPT
$script_trusty_test32 = <<SCRIPT
SCRIPT
$script_precise32 = <<SCRIPT
dpkg --add-architecture i386
apt-key adv --fetch-keys http://repos.codelite.org/CodeLite.asc
apt-add-repository 'deb http://repos.codelite.org/wx3.0-1/ubuntu/ precise universe'
add-apt-repository ppa:kalakris/cmake
apt-get update
apt-get install -y libwxgtk3.0-dev libboost-dev cmake build-essential libc6-dev build-essential cpp dkms g++ gcc gcc-multilib virtualbox-guest-dkms libf2c2 libf2c2-dev f2c
SCRIPT
$script_precise_test32 = <<SCRIPT
apt-key adv --fetch-keys http://repos.codelite.org/CodeLite.asc
apt-add-repository 'deb http://repos.codelite.org/wx3.0-1/ubuntu/ precise universe'
apt-get update
SCRIPT
$script_trusty = <<SCRIPT
dpkg --add-architecture i386
apt-get update
apt-get install -y libwxgtk3.0-dev libboost-dev cmake build-essential libc6-dev libc6-dev-i386 multiarch-support build-essential cpp dkms g++ gcc gcc-multilib virtualbox-guest-dkms libf2c2:i386 libf2c2-dev:i386 f2c:i386
SCRIPT
$script_trusty_test = <<SCRIPT
SCRIPT
$script_precise = <<SCRIPT
dpkg --add-architecture i386
apt-key adv --fetch-keys http://repos.codelite.org/CodeLite.asc
apt-add-repository 'deb http://repos.codelite.org/wx3.0-1/ubuntu/ precise universe'
add-apt-repository ppa:kalakris/cmake
apt-get update
apt-get install -y libwxgtk3.0-dev libboost-dev cmake build-essential libc6-dev libc6-dev-i386 multiarch-support build-essential cpp dkms g++ gcc gcc-multilib virtualbox-guest-dkms libf2c2:i386 libf2c2-dev:i386 f2c:i386
SCRIPT
$script_precise_test = <<SCRIPT
apt-key adv --fetch-keys http://repos.codelite.org/CodeLite.asc
apt-add-repository 'deb http://repos.codelite.org/wx3.0-1/ubuntu/ precise universe'
apt-get update
SCRIPT
Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  # All Vagrant configuration is done here. The most common configuration
  # options are documented and commented below. For a complete reference,
  # please see the online documentation at vagrantup.com.
  config.vm.define "ubuntu32" do |ubuntu32|
  # Every Vagrant virtual environment requires a box to build off of.
    ubuntu32.vm.box = "ubuntu/trusty32"
    ubuntu32.vm.provision "shell", inline: $script_trusty32
  end
  config.vm.define "ubuntu32test" do |ubuntu32test|
  # Every Vagrant virtual environment requires a box to build off of.
    ubuntu32test.vm.box = "ubuntu/trusty32"
    ubuntu32test.vm.provision "shell", inline: $script_trusty_test32
  end
  config.vm.define "ubuntu32precise" do |ubuntu32precise|
  # Every Vagrant virtual environment requires a box to build off of.
    ubuntu32precise.vm.box = "ubuntu/precise32"
    ubuntu32precise.vm.provision "shell", inline: $script_precise32
  end
  config.vm.define "ubuntu32precisetest" do |ubuntu32precisetest|
  # Every Vagrant virtual environment requires a box to build off of.
    ubuntu32precisetest.vm.box = "ubuntu/precise32"
    ubuntu32precisetest.vm.provision "shell", inline: $script_precise_test32
  end
  config.vm.define "ubuntu64" do |ubuntu64|
  # Every Vagrant virtual environment requires a box to build off of.
    ubuntu64.vm.box = "ubuntu/trusty64"
    ubuntu64.vm.provision "shell", inline: $script_trusty
  end
  config.vm.define "ubuntu64test" do |ubuntu64test|
  # Every Vagrant virtual environment requires a box to build off of.
    ubuntu64test.vm.box = "ubuntu/trusty64"
    ubuntu64test.vm.provision "shell", inline: $script_trusty_test
  end
  config.vm.define "ubuntu64precise" do |ubuntu64precise|
  # Every Vagrant virtual environment requires a box to build off of.
    ubuntu64precise.vm.box = "ubuntu/precise64"
    ubuntu64precise.vm.provision "shell", inline: $script_precise
  end
  config.vm.define "ubuntu64precisetest" do |ubuntu64precisetest|
  # Every Vagrant virtual environment requires a box to build off of.
    ubuntu64precisetest.vm.box = "ubuntu/precise64"
    ubuntu64precisetest.vm.provision "shell", inline: $script_precise_test
  end
  config.vm.define "windows7" do |windows7|
    windowsxp.vm.communication = "winrm"
  end
  # Disable automatic box update checking. If you disable this, then
  # boxes will only be checked for updates when the user runs
  # `vagrant box outdated`. This is not recommended.
  # config.vm.box_check_update = false

  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine. In the example below,
  # accessing "localhost:8080" will access port 80 on the guest machine.
  # config.vm.network "forwarded_port", guest: 80, host: 8080

  # Create a private network, which allows host-only access to the machine
  # using a specific IP.
  # config.vm.network "private_network", ip: "192.168.33.10"

  # Create a public network, which generally matched to bridged network.
  # Bridged networks make the machine appear as another physical device on
  # your network.
  # config.vm.network "public_network"

  # If true, then any SSH connections made will enable agent forwarding.
  # Default value: false
  # config.ssh.forward_agent = true
  config.ssh.forward_x11 = true

  # Share an additional folder to the guest VM. The first argument is
  # the path on the host to the actual folder. The second argument is
  # the path on the guest to mount the folder. And the optional third
  # argument is a set of non-required options.
  # config.vm.synced_folder "../data", "/vagrant_data"

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  #
  # config.vm.provider "virtualbox" do |vb|
  #   # Don't boot with headless mode
  #   vb.gui = true
  #
  #   # Use VBoxManage to customize the VM. For example to change memory:
  #   vb.customize ["modifyvm", :id, "--memory", "1024"]
  # end
  #
  # View the documentation for the provider you're using for more
  # information on available options.

  # Enable provisioning with CFEngine. CFEngine Community packages are
  # automatically installed. For example, configure the host as a
  # policy server and optionally a policy file to run:
  #
  # config.vm.provision "cfengine" do |cf|
  #   cf.am_policy_hub = true
  #   # cf.run_file = "motd.cf"
  # end
  #
  # You can also configure and bootstrap a client to an existing
  # policy server:
  #
  # config.vm.provision "cfengine" do |cf|
  #   cf.policy_server_address = "10.0.2.15"
  # end

  # Enable provisioning with Puppet stand alone.  Puppet manifests
  # are contained in a directory path relative to this Vagrantfile.
  # You will need to create the manifests directory and a manifest in
  # the file default.pp in the manifests_path directory.
  #
  # config.vm.provision "puppet" do |puppet|
  #   puppet.manifests_path = "manifests"
  #   puppet.manifest_file  = "site.pp"
  # end

  # Enable provisioning with chef solo, specifying a cookbooks path, roles
  # path, and data_bags path (all relative to this Vagrantfile), and adding
  # some recipes and/or roles.
  #
  # config.vm.provision "chef_solo" do |chef|
  #   chef.cookbooks_path = "../my-recipes/cookbooks"
  #   chef.roles_path = "../my-recipes/roles"
  #   chef.data_bags_path = "../my-recipes/data_bags"
  #   chef.add_recipe "mysql"
  #   chef.add_role "web"
  #
  #   # You may also specify custom JSON attributes:
  #   chef.json = { mysql_password: "foo" }
  # end

  # Enable provisioning with chef server, specifying the chef server URL,
  # and the path to the validation key (relative to this Vagrantfile).
  #
  # The Opscode Platform uses HTTPS. Substitute your organization for
  # ORGNAME in the URL and validation key.
  #
  # If you have your own Chef Server, use the appropriate URL, which may be
  # HTTP instead of HTTPS depending on your configuration. Also change the
  # validation key to validation.pem.
  #
  # config.vm.provision "chef_client" do |chef|
  #   chef.chef_server_url = "https://api.opscode.com/organizations/ORGNAME"
  #   chef.validation_key_path = "ORGNAME-validator.pem"
  # end
  #
  # If you're using the Opscode platform, your validator client is
  # ORGNAME-validator, replacing ORGNAME with your organization name.
  #
  # If you have your own Chef Server, the default validation client name is
  # chef-validator, unless you changed the configuration.
  #
  #   chef.validation_client_name = "ORGNAME-validator"
end
