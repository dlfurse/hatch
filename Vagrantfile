Vagrant.configure("2") do |config|
  
  config.vm.box = "rueian/ubuntu20-m1"
  config.vm.hostname = "nest"
  config.vm.network "private_network", ip: "192.168.10.10"
  config.vm.network "forwarded_port", id: "ssh", host: 2222, guest: 22, auto_correct: false
  config.vm.synced_folder ".", "/vagrant", disabled: true
  
  config.vm.provider "parallels" do |prl|
    prl.name = "nest"
    prl.update_guest_tools = true
    prl.memory = 2048
    prl.cpus = 2
  end

  config.vm.provision "shell", inline: <<-EOF
    sudo apt-get update
    sudo apt-get install -y build-essential cmake
    sudo ln -s /usr/bin/gcc /usr/bin/c++
  EOF

end
