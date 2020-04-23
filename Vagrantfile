Vagrant.configure("2") do |config|
  config.vm.box = "centos/8"
  config.vm.hostname = "nest"
  config.vm.network "private_network", ip: "192.168.10.10"
  config.vm.network "forwarded_port", id: "ssh", host: 2222, guest: 22, auto_correct: false
  config.vm.synced_folder ".", "/vagrant", disabled: true

  config.vm.provider :virtualbox do |vb|
    vb.customize ["modifyvm", :id, "--memory", "2048"]
    vb.customize ["modifyvm", :id, "--cpus", "4"]
    vb.customize ["modifyvm", :id, "--natdnshostresolver1", "on"]
    vb.customize ["modifyvm", :id, "--natdnsproxy1", "on"]
    vb.customize ["guestproperty", "set", :id, "/VirtualBox/GuestAdd/VBoxService/--timesync-set-threshold", 10000]
  end

  config.vm.provision "shell", inline: <<-EOF
    sudo useradd -o -u 501 -g games egg
    sudo yum -y update
    sudo yum install -y centos-release-stream
    sudo yum install -y epel-release
    sudo yum install -y gcc-c++
    sudo yum install -y gdb
    sudo yum install -y git
    sudo yum install -y patch
    sudo yum install -y make
    sudo yum install -y cmake3
    sudo yum install -y automake
    sudo yum install -y libtool
    sudo yum install -y python3
    sudo yum install -y perl
    
  EOF

  config.vbguest.installer = VagrantVbguest::Installers::Linux
  config.disksize.size = '50GB'
end
