@Table
public class acl
{
    @PrimaryKey
    public String peer_pubkey;
    @Column
    public int type; // KICKLEVEL_INVALID = 0, KICKLEVEL_MUTE = 1, KICKLEVEL_KICK = 2
}
