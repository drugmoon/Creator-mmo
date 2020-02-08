module(..., package.seeall)
SIAP_MAXHP_MUL=1;
SIAP_MAXHP_ADD=2;
SIAP_MAXMP_MUL=3;
SIAP_MAXMP_ADD=4;
SIAP_AC_MUL=5;
SIAP_AC_ADD=6;
SIAP_ACMAX_MUL=7;
SIAP_ACMAX_ADD=8;
SIAP_MAC_MUL=9;
SIAP_MAC_ADD=10;
SIAP_MACMAX_MUL=11;
SIAP_MACMAX_ADD=12;
SIAP_DC_MUL=13;
SIAP_DC_ADD=14;
SIAP_DCMAX_MUL=15;
SIAP_DCMAX_ADD=16;
SIAP_MC_MUL=17;
SIAP_MC_ADD=18;
SIAP_MCMAX_MUL=19;
SIAP_MCMAX_ADD=20;
SIAP_SC_MUL=21;
SIAP_SC_ADD=22;
SIAP_SCMAX_MUL=23;
SIAP_SCMAX_ADD=24;
SIAP_FIGHT=25;
SIAP_SUB_HARM=26;
SIAPNUM=26;

ITEM_WEAPON_POSITION = (-2*2);
ITEM_CLOTH_POSITION = (-3*2);
ITEM_HAT_POSITION = (-4*2);
ITEM_RING1_POSITION = (-5*2);
ITEM_RING2_POSITION = (-5*2-1);
ITEM_GLOVE1_POSITION = (-6*2);
ITEM_GLOVE2_POSITION = (-6*2-1);
ITEM_NICKLACE_POSITION = (-7*2);
ITEM_HUIZHANG_POSITION = (-8*2);
ITEM_BELT_POSITION = (-9*2);
ITEM_BOOT_POSITION = (-10*2);
ITEM_GUANZHI_POSITION = (-11*2);
ITEM_XUESHI_POSITION = (-12*2);
ITEM_WING_POSITION = (-13*2);
ITEM_FASHION_POSITION = (-15 * 2);
ITEM_MOUNT_HUJIA	=(-16*2);
ITEM_MOUNT_JIANGSHEN=(-17*2);
ITEM_MOUNT_MAAN		=(-18*2);
ITEM_MOUNT_TITIE	=(-19*2);

local siap_data={};
function add_siap_listener(pos,fun)
	if not siap_data[pos] then
		siap_data[pos] = {};
	end
	local d = siap_data[pos];
	d[fun] = fun;
end
function fresh_siap_attr(player,pos)
	local d = siap_data[pos];
	if d then
		local attr={};for i=1,SIAPNUM do attr[i]=0;end;
		for i,v in pairs(d) do
			v(player,attr,pos);
		end
		player:set_script_item_attr(pos,unpack(attr));
	end
end

function update_attr(player)
	player:recal_attr();
end

function attr_all_in_one(player)
	fresh_siap_attr(player);
	fresh_siap_attr(player,ITEM_WEAPON_POSITION);
	fresh_siap_attr(player,ITEM_CLOTH_POSITION);
	fresh_siap_attr(player,ITEM_HAT_POSITION);
	fresh_siap_attr(player,ITEM_RING1_POSITION);
	fresh_siap_attr(player,ITEM_RING2_POSITION);
	fresh_siap_attr(player,ITEM_GLOVE1_POSITION);
	fresh_siap_attr(player,ITEM_GLOVE2_POSITION);
	fresh_siap_attr(player,ITEM_NICKLACE_POSITION);
	fresh_siap_attr(player,ITEM_HUIZHANG_POSITION);
	fresh_siap_attr(player,ITEM_BELT_POSITION);
	fresh_siap_attr(player,ITEM_BOOT_POSITION);
	fresh_siap_attr(player,ITEM_GUANZHI_POSITION);
	fresh_siap_attr(player,ITEM_WING_POSITION);
	fresh_siap_attr(player,ITEM_FASHION_POSITION);

	fresh_siap_attr(player,ITEM_MOUNT_HUJIA);
	fresh_siap_attr(player,ITEM_MOUNT_JIANGSHEN);
	fresh_siap_attr(player,ITEM_MOUNT_MAAN);
	fresh_siap_attr(player,ITEM_MOUNT_TITIE);
	update_attr(player);
end
login.add_login_listener(attr_all_in_one);
