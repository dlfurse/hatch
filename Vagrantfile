Vagrant.configure("2") do |cfg|
  
  cfg.vm.box = "rueian/ubuntu20-m1"
  cfg.vm.hostname = "nest"
  cfg.vm.network "private_network", ip: "192.168.10.10"
  cfg.vm.network "forwarded_port", id: "ssh", host: 2222, guest: 22, auto_correct: false
  cfg.vm.synced_folder ".", "/vagrant", disabled: true
  
  cfg.vm.provider "parallels" do |prl|
    prl.name = "nest"
    prl.update_guest_tools = true
    prl.memory = 2048
    prl.cpus = 2
  end

  cfg.vm.provision "shell", inline: <<-EOF
    sudo apt-get update
    sudo apt-get install -y build-essential cmake
  EOF

end
