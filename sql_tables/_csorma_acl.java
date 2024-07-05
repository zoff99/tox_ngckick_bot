@Table
public class acl
{
    @PrimaryKey
    public String peer_pubkey;
    @Column
    public int type; // 0 -> mute, 1 -> kick
}
