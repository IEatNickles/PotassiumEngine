#pragma once

#include "Scene.hpp"
#include <cstddef>

struct RelationshipComponent {
  size_t child_count = 0;
  entt::entity first = entt::null;
  entt::entity last = entt::null;
  entt::entity prev = entt::null;
  entt::entity next = entt::null;
  entt::entity parent = entt::null;

  bool add_child(entt::registry &reg, entt::entity e, entt::entity child) {
    if (child == entt::null || child == e || has_parent(reg, child)) {
      return false;
    }

    if (child_count == 0) {
      first = child;
    } else {
      entt::entity curr = first;
      for (size_t i = 0; i < child_count; ++i) {
        if (curr == child) {
          return false;
        }
        curr = reg.get<RelationshipComponent>(curr).next;
      }
    }

    auto &cr = reg.get_or_emplace<RelationshipComponent>(child);
    if (cr.parent != entt::null) {
      reg.get<RelationshipComponent>(cr.parent).remove_child(reg, child);
    }
    cr.parent = e;

    cr.next = entt::null;
    cr.prev = last;
    if (last != entt::null) {
      reg.get<RelationshipComponent>(last).next = child;
    }
    last = child;

    ++child_count;

    return true;
  }

  bool remove_child(entt::registry &reg, entt::entity child) {
    if (child == entt::null) {
      return false;
    }
    entt::entity curr = first;
    for (size_t i = 0; i < child_count; ++i) {
      if (curr == child) {
        entt::entity p = reg.get<RelationshipComponent>(curr).prev;
        entt::entity n = reg.get<RelationshipComponent>(curr).next;
        if (curr == first) {
          first = n;
        }
        if (curr == last) {
          last = p;
        }
        if (p != entt::null) {
          reg.get<RelationshipComponent>(p).next = n;
        }
        if (n != entt::null) {
          reg.get<RelationshipComponent>(n).prev = p;
        }
        --child_count;
        if (child_count == 0 && parent == entt::null) {
          reg.remove<RelationshipComponent>(
              reg.get<RelationshipComponent>(child).parent);
        }
        return true;
      }
      curr = reg.get<RelationshipComponent>(curr).next;
    }
    return false;
  }

  bool has_parent(entt::registry &reg, entt::entity p) const {
    if (parent == entt::null) {
      return false;
    }

    entt::entity curr = parent;
    while (curr != entt::null) {
      if (curr == p) {
        return true;
      }
      curr = reg.get<RelationshipComponent>(curr).parent;
    }

    return false;
  }
};
