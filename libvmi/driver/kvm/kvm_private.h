
/* The LibVMI Library is an introspection library that simplifies access to
 * memory in a target virtual machine or in a file containing a dump of
 * a system's physical memory.  LibVMI is based on the XenAccess Library.
 *
 * Copyright 2011 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government
 * retains certain rights in this software.
 *
 * Author: Bryan D. Payne (bdpayne@acm.org)
 * Author: Tamas K Lengyel (tamas.lengyel@zentific.com)
 *
 * This file is part of LibVMI.
 *
 * LibVMI is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * LibVMI is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with LibVMI.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KVM_PRIVATE_H
#define KVM_PRIVATE_H

#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

#include "private.h"
#include "libvirt_wrapper.h"
#include "driver/kvm/include/kvmi/libkvmi.h"

typedef struct kvm_instance {
    virConnectPtr conn;
    virDomainPtr dom;
    uint32_t id;
    char *name;
    char *ds_path;
    libvirt_wrapper_t libvirt;
    void *kvmi;
    void *kvmi_dom;
    pthread_mutex_t kvm_connect_mutex;
    pthread_cond_t kvm_start_cond;
    unsigned int expected_pause_count;
    // store KVMI_EVENT_PAUSE_VCPU events poped by vmi_events_listen(vmi, 0)
    // to be used by vmi_resume_vm()
    struct kvmi_dom_event** pause_events_list;
    bool vcpu_sstep[16]; // whether singlestepping is enabled on a given vcpu
    // dispatcher to handle VM events in each process_xxx functions
    status_t (*process_event[KVMI_NUM_EVENTS])(vmi_instance_t vmi, struct kvmi_dom_event *event);
} kvm_instance_t;

static inline kvm_instance_t *
kvm_get_instance(
    vmi_instance_t vmi)
{
    return ((kvm_instance_t *) vmi->driver.driver_data);
}

#endif
