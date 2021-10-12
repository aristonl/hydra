/*
 * Device will be the base class of everything in /dev.
 *
 * There are 2 subclasses
 *  - BlockDevice (random access, eg: Hard Drives, GPUs, RAM, etc)
 *  - CharacterDevices (sequential, eg: serial console, etc)
 */

#pragma once

#include "../FileSystem/ext2/File.h"
#include "../../../std/UnixTypes.h"

class Device : public File {
public:
    virtual ~Device() override;

    unsigned major() const { return m_major; }
    unsigned minor() const { return m_minor; }

    virtual String absolute_path(const FileDescription&) const override;
    virtual String absolute_path() const;

    uid_t uid() const { return m_uid; }
    uid_t gid() const { return m_gid; }

    virtual bool is_device() const override { return true; }
    virtual bool is_disk_device() const { return false; }

    static void for_each(Function<void(Device&)>);
    static Device* get_device(unsigned major, unsigned minor);

protected:
    Device(unsigned major, unsigned minor);
    void set_uid(uid_t uid) { m_uid = uid; }
    void set_gid(gid_t gid) { m_gid = gid; }

private:
    unsigned m_major { 0 };
    unsigned m_minor { 0 };
    uid_t m_uid { 0 };
    gid_t m_gid { 0 };
};